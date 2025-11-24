#include <ArduinoJson.h>
#include <Wire.h>
#ifndef I2C_SDA
#define I2C_SDA SDA
#endif
#ifndef I2C_SCL
#define I2C_SCL SCL
#endif

#include "CWS.h"
CWS input;

void setup() {
    Serial.begin(115200);
    delay(1000);
    Serial.printf("\nCWS JSON Test\n");

    Wire.setPins(I2C_SDA, I2C_SCL);
    Wire.begin();

    if (input.begin()) {
        Serial.println("CWS initialized successfully.");
    } else {
        Serial.println("Failed to initialize CWS!");
        delay(10000);
        exit(0);
    }
}

void loop() {
    JsonDocument root;

    if (input.getJSON(root)) {
        serializeJsonPretty(root, Serial);
        Serial.println();
    } else {
        Serial.println("Failed to get CWS data.");
    }

    delay(1000);
}