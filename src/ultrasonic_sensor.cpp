/**
 * @file ultrasonic_sensor.cpp
 * @brief Implementação da classe UltrasonicSensor.
 * @details Contém a implementação dos métodos de configuração e leitura do sensor
 *          ultrassônico HC-SR04. Os pinos e constantes são membros privados da classe.
 */

#include "../include/ultrasonic_sensor.h"

void UltrasonicSensor::begin(uint8_t trig, uint8_t echo) {
    m_trigPin = trig;
    m_echoPin = echo;
    pinMode(m_trigPin, OUTPUT);
    pinMode(m_echoPin, INPUT_PULLUP);
}

/**
 * @brief Implementação da função de leitura de distância.
 * @details Executa o ciclo de medição 5 vezes, com intervalo de 1 segundo entre
 *          cada leitura, para obter uma medição mais estável.
 *          1. Itera 5 vezes para coletar as amostras.
 *          2. Em cada iteração, envia um pulso de trigger e mede a duração do eco.
 *          3. Converte a duração em distância. Leituras inválidas são descartadas.
 *          4. Calcula a média das leituras válidas.
 * @return A distância média medida em cm, ou -1 se todas as leituras falharem.
 */
long UltrasonicSensor::lerDistancia() {
    long total_distance = 0;
    int valid_readings = 0;

    for (int i = 0; i < 5; i++) {
        digitalWrite(m_trigPin, LOW);
        delayMicroseconds(2);
        digitalWrite(m_trigPin, HIGH);
        delayMicroseconds(TRIGGER_PULSE_DURATION_US);
        digitalWrite(m_trigPin, LOW);

        long duration = pulseIn(m_echoPin, HIGH, PULSEIN_TIMEOUT_US);
        long distance = (duration == 0) ? -1 : duration / SOUND_SPEED_DIVISOR;

        if (distance != -1) {
            total_distance += distance;
            valid_readings++;
        }
        delay(1000); // Espera 1 segundo entre as leituras
    }

    if (valid_readings > 0) {
        Serial.println("Distancia media: " + String(total_distance / valid_readings) + " cm");
        return total_distance / valid_readings;
    } else {
        return -1; // Retorna -1 se todas as leituras falharem
    }
}