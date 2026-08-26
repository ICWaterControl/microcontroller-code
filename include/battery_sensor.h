#ifndef BATTERY_SENSOR_H
#define BATTERY_SENSOR_H

#include <Wire.h>
#include <SparkFun_MAX1704x_Fuel_Gauge_Arduino_Library.h>

// Driver para o fuel gauge MAX1704x
class BatterySensor {
public:
    BatterySensor() = default;

    // Inicializa I2C e o sensor de bateria
    void begin();

    // Coloca o sensor em sleep e encerra I2C
    void sleep();

    // Lê porcentagem e voltagem da bateria
    void lerDados(float& percentage, float& voltage);

private:
    SFE_MAX1704X _fuelGauge;
    static constexpr int SDA_PIN = 21;
    static constexpr int SCL_PIN = 22;
};

#endif
