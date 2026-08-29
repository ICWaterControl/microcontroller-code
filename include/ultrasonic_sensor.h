#ifndef ULTRASONIC_SENSOR_H
#define ULTRASONIC_SENSOR_H

#include <Arduino.h>

// Driver para o sensor ultrassônico HC-SR04
class UltrasonicSensor {
public:
    UltrasonicSensor() = default;

    // Configura os pinos de trigger e echo do sensor
    void begin(uint8_t trig, uint8_t echo);

    // Realiza 5 medições e retorna a média em cm, ou -1 se todas falharem
    long lerDistancia();

private:
    uint8_t m_trigPin = 0;
    uint8_t m_echoPin = 0;

    static constexpr int TRIGGER_PULSE_DURATION_US = 15;
    static constexpr int PULSEIN_TIMEOUT_US = 26000;
    static constexpr float SOUND_SPEED_DIVISOR = 58.0f;
};

#endif