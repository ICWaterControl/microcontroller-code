#include "../include/battery_sensor.h"
#include <Arduino.h>
#include <Wire.h>
#include <SparkFun_MAX1704x_Fuel_Gauge_Arduino_Library.h>

// Instância do objeto do sensor
static SFE_MAX1704X fuelGauge;

// Pinos I2C
static const int SDA_PIN = 21;
static const int SCL_PIN = 22;

void setupBatterySensor() {
    Wire.begin(SDA_PIN, SCL_PIN);
    Serial.println("Initializing Battery Sensor...");
    
    if (fuelGauge.begin() == false) {
        Serial.println("Error: Battery Sensor MAX1704x not found. Please check wiring.");
    } else {
        Serial.println("Battery Sensor MAX1704x initialized.");
    }
}

void lerDadosBateria(float& percentage, float& voltage) {
    percentage = fuelGauge.getSOC();
    voltage = fuelGauge.getVoltage();
}
