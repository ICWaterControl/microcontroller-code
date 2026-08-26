/**
 * @file battery_sensor.cpp
 * @brief Implementação da classe BatterySensor.
 * @details Contém a lógica de inicialização I2C, leitura de SOC/voltagem
 *          e modo sleep do fuel gauge MAX1704x.
 */

#include "../include/battery_sensor.h"
#include <Arduino.h>

void BatterySensor::begin() {
    Wire.begin(SDA_PIN, SCL_PIN);

    if (_fuelGauge.begin() == false) {
        return;
    }
}

void BatterySensor::sleep() {
    _fuelGauge.sleep();
    Wire.end();
}

void BatterySensor::lerDados(float& percentage, float& voltage) {
    percentage = _fuelGauge.getSOC();
    voltage = _fuelGauge.getVoltage();

    Serial.printf("[BATTERY] Percentage: %.2f | Voltage: %.2f\n", percentage, voltage);
}
