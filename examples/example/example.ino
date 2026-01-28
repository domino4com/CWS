#include <ArduinoJson.h>
#include <Wire.h>
#ifndef I2C_SDA
#define I2C_SDA SDA
#endif
#ifndef I2C_SCL
#define I2C_SCL SCL
#endif
#include <CWS.h>

CWS input;
float  illuminance, light_voltage, x, y, z, magnitude, inclination;
char s[] = "Lux: %.0f lx (Light mV: %.0f mV), X: %.3f g, Y: %.3f g, Z: %.3f g, Mag: %.2f g, Tilt: %.0f °\n";  // Fixed: mV label, %.0f for integers

void setup() {
    Serial.begin(115200);
    delay(1000);
    Serial.printf("\nCWS Test\n");

    Wire.setPins(I2C_SDA, I2C_SCL);
    Wire.begin();

    if (input.begin()) {
        Serial.println("CWS sensor initialized successfully.");
    } else {
        Serial.println("Failed to initialize CWS sensor!");
        exit(0);
    }
}

void loop() {
    if (input.getData(illuminance, light_voltage, x, y, z, magnitude, inclination)) {
        Serial.printf(s, illuminance, light_voltage, x, y, z, magnitude, inclination);
    } else {
        Serial.println("Failed to CWS data.");
    }

    delay(1000);
}