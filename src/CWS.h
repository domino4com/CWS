#ifndef CWS_H
#define CWS_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include <Wire.h>
#include <esp_adc_cal.h>
#include <math.h>

// Fixed pins for CWS board (ESP32-S2)
#define CWS_SDA 5
#define CWS_SCL 4
#define CWS_NTC_PIN 14  // ADC2_CHANNEL_3
#define CWS_LIGHT_PIN 16 // ADC2_CHANNEL_5

// KX022 Accelerometer (I2C)
#define CWS_ACCEL_ADDR 0x1F
#define CWS_ACCEL_WHO_AM_I 0x0F
#define CWS_ACCEL_CNTL1 0x18
#define CWS_ACCEL_ODCNTL 0x1B
#define CWS_ACCEL_DATA_CTRL 0x21
#define CWS_ACCEL_OUTX_L 0x06
#define CWS_ACCEL_EXPECTED_WHOAMI 0x14

// NTC Steinhart-Hart coefficients
#define CWS_SH_A 6.718384e-04f
#define CWS_SH_B 2.912598e-04f
#define CWS_SH_C -5.411298e-10f
#define CWS_R_SERIES 10000.0f

// ADC settings
#define CWS_ADC_UNIT ADC_UNIT_2
#define CWS_ADC_WIDTH ADC_WIDTH_BIT_13
#define CWS_ADC_ATTEN ADC_ATTEN_DB_12  // Updated: DB_11 deprecated, use DB_12
#define CWS_SAMPLES 10

class CWS {
public:
  CWS();
  bool begin();
  bool getData(float &temperature, float &ntc_voltage, float &illuminance, float &light_voltage, float &x, float &y, float &z, float &magnitude, float &inclination);
  bool getJSON(JsonDocument &doc);
  void calibrateAccel();  // Set zero orientation

private:
  esp_adc_cal_characteristics_t _adc_chars_ntc;
  esp_adc_cal_characteristics_t _adc_chars_light;
  float _ax0, _ay0;

  void _writeRegister(uint8_t reg, uint8_t value);
  uint8_t _readRegister(uint8_t reg);
  void _readRawAccel(int16_t &x, int16_t &y, int16_t &z);
  float _readADC(uint8_t channel);
  float _readTemperature();
  float _readNTCVoltage();
  float _readLight();
  float _readLightVoltage();
  bool _readAcceleration(float &x, float &y, float &z);
  float _readAccelerationMagnitude(float x, float y, float z);  // Fixed: param-based
  float _readInclination();
};

#endif  // CWS_H