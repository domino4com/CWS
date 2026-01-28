#include "CWS.h"

CWS::CWS() : _ax0(0.0f), _ay0(0.0f) {}

bool CWS::begin() {
    Wire.begin(CWS_SDA, CWS_SCL);
    delay(100);

    // ADC Light
    esp_adc_cal_value_t val_type = esp_adc_cal_characterize(CWS_ADC_UNIT, CWS_ADC_ATTEN, CWS_ADC_WIDTH, 1125, &_adc_chars_light);  // 1125 or 1100
    adc1_config_channel_atten(CWS_ADC_CHANNEL, CWS_ADC_ATTEN);
    pinMode(CWS_LIGHT_PIN, INPUT);
    adc1_config_width(CWS_ADC_WIDTH);  // 12 bits seems too jittery, 11 bits work just fine.

    // Accel init
    if (_readRegister(CWS_ACCEL_WHO_AM_I) != CWS_ACCEL_EXPECTED_WHOAMI) return false;
    _writeRegister(CWS_ACCEL_CNTL1, 0x00);
    _writeRegister(CWS_ACCEL_ODCNTL, 0x02);  // 50Hz
    _writeRegister(CWS_ACCEL_DATA_CTRL, 0x02);
    _writeRegister(CWS_ACCEL_CNTL1, 0xC0);  // On

    calibrateAccel();
    return true;
}

void CWS::calibrateAccel() {
    int16_t ix, iy, iz;
    _readRawAccel(ix, iy, iz);
    _ax0 = (float)ix;
    _ay0 = (float)iy;
}

bool CWS::getData(float& illuminance, float& light_voltage, float& x, float& y, float& z, float& magnitude, float& inclination) {
    illuminance = _readLight();
    light_voltage = _readLightVoltage();
    if (!_readAcceleration(x, y, z)) return false;
    magnitude = _readAccelerationMagnitude(x, y, z);
    inclination = _readInclination();
    return true;
}

bool CWS::getJSON(JsonDocument& doc) {
    float illuminance = _readLight();
    float light_voltage = _readLightVoltage();
    float x, y, z;
    if (!_readAcceleration(x, y, z)) return false;
    float magnitude = _readAccelerationMagnitude(x, y, z);
    float inclination = _readInclination();

    JsonArray dataArray = doc["CWS"].to<JsonArray>();

    JsonObject dataSet = dataArray.add<JsonObject>();  // First data set
    dataSet["name"] = "Illuminance";
    dataSet["value"] = illuminance;
    dataSet["unit"] = "lx";

    dataSet = dataArray.add<JsonObject>();  // Subsequent data sets
    dataSet["name"] = "Phototransistor Voltage";
    dataSet["value"] = light_voltage;
    dataSet["unit"] = "mV";  // Fixed: mV for accuracy

    dataSet = dataArray.add<JsonObject>();  // Subsequent data sets
    dataSet["name"] = "X";
    dataSet["value"] = x;
    dataSet["unit"] = "g";

    dataSet = dataArray.add<JsonObject>();  // Subsequent data sets
    dataSet["name"] = "Y";
    dataSet["value"] = y;
    dataSet["unit"] = "g";

    dataSet = dataArray.add<JsonObject>();  // Subsequent data sets
    dataSet["name"] = "Z";
    dataSet["value"] = z;
    dataSet["unit"] = "g";

    dataSet = dataArray.add<JsonObject>();  // Subsequent data sets
    dataSet["name"] = "Magnitude";
    dataSet["value"] = magnitude;
    dataSet["unit"] = "g";

    dataSet = dataArray.add<JsonObject>();  // Subsequent data sets
    dataSet["name"] = "Inclination";
    dataSet["value"] = inclination;
    dataSet["unit"] = "°";

    return true;
}

float CWS::_readLight() {
    float voltage = _readLightVoltage();
    if (voltage <= 0) return 0.0f;
    float resistance = CWS_R_SERIES * (3300.0f / voltage - 1.0f);  // Expects mV
    if (resistance <= 0) return 0.0f;
    return 500.0f / (resistance / 1000.0f);
}

float CWS::_readLightVoltage() {
    return _readADC(CWS_ADC_CHANNEL);
}

bool CWS::_readAcceleration(float& x, float& y, float& z) {
    int16_t rx, ry, rz;
    _readRawAccel(rx, ry, rz);
    x = (float)rx / 16384.0f;
    y = (float)ry / 16384.0f;
    z = (float)rz / 16384.0f;
    return true;
}

float CWS::_readAccelerationMagnitude(float x, float y, float z) {
    return sqrt(x * x + y * y + z * z);
}

float CWS::_readInclination() {
    int16_t ax, ay, dummy;
    _readRawAccel(ax, ay, dummy);
    float a_ax = (float)ax;
    float a_ay = (float)ay;

    float dot = _ax0 * a_ax + _ay0 * a_ay;
    float cross = _ax0 * a_ay - _ay0 * a_ax;
    float maga = sqrt(a_ax * a_ax + a_ay * a_ay);
    float mag0 = sqrt(_ax0 * _ax0 + _ay0 * _ay0);

    float norm_dot = (maga > 0 && mag0 > 0) ? dot / (maga * mag0) : 1.0f;
    float norm_cross = (maga > 0 && mag0 > 0) ? cross / (maga * mag0) : 0.0f;
    norm_dot = constrain(norm_dot, -1.0f, 1.0f);

    return atan2(norm_cross, norm_dot) * 180.0f / M_PI;
}

void CWS::_writeRegister(uint8_t reg, uint8_t value) {
    Wire.beginTransmission(CWS_ACCEL_ADDR);
    Wire.write(reg);
    Wire.write(value);
    Wire.endTransmission();
}

uint8_t CWS::_readRegister(uint8_t reg) {
    Wire.beginTransmission(CWS_ACCEL_ADDR);
    Wire.write(reg);
    Wire.endTransmission(false);
    Wire.requestFrom(CWS_ACCEL_ADDR, 1);
    return Wire.read();
}

void CWS::_readRawAccel(int16_t& x, int16_t& y, int16_t& z) {
    Wire.beginTransmission(CWS_ACCEL_ADDR);
    Wire.write(CWS_ACCEL_OUTX_L);
    Wire.endTransmission(false);
    Wire.requestFrom(CWS_ACCEL_ADDR, 6);
    x = (int16_t)(Wire.read() | (Wire.read() << 8));
    y = (int16_t)(Wire.read() | (Wire.read() << 8));
    z = (int16_t)(Wire.read() | (Wire.read() << 8));
}

float CWS::_readADC(uint8_t channel) {
    int sum = 0;
    for (int i = 0; i < CWS_SAMPLES; i++) {
        int raw;
        // if (adc1_get_raw((adc1_channel_t)channel, CWS_ADC_WIDTH, &raw) == ESP_OK) sum += raw;
        sum += adc1_get_raw((adc1_channel_t)channel);
        delay(1);
    }
    int avg_raw = sum / CWS_SAMPLES;
    if (avg_raw < 0 || avg_raw > 8191) return 0.0f;
    return (float)esp_adc_cal_raw_to_voltage(avg_raw, &_adc_chars_light);  // Returns mV directly
}