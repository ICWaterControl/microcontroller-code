/**
 * @file ultrasonic_sensor.cpp
 * @brief Implementação do driver e da lógica de publicação para o sensor ultrassônico.
 * @details Este arquivo contém a implementação das funções que controlam o sensor ultrassônico HC-SR04.
 *          Ele lida com a configuração dos pinos, a execução da medição de distância e a subsequente
 *          publicação desses dados. A lógica de publicação é robusta, enviando os dados via MQTT quando
 *          conectado ou registrando-os localmente através do `publish_manager` em caso de falha na conexão.
 */

#include "../include/ultrasonic_sensor.h"
#include <Arduino.h>

// --- Configuração e Constantes ---

// Pinos do microcontrolador conectados ao sensor.
static int trigPin, echoPin;

// Duração do pulso de trigger em microssegundos. Um pulso mais longo pode garantir a ativação em alguns sensores.
const int TRIGGER_PULSE_DURATION_US = 15;
// Timeout para a função pulseIn. Define o tempo máximo de espera pelo eco, o que determina o alcance máximo da medição.
const int PULSEIN_TIMEOUT_US = 26000;
// Divisor para converter a duração do eco (em microssegundos) para centímetros. Baseado na velocidade do som.
const float SOUND_SPEED_DIVISOR = 58.0;

/**
 * @brief Implementação da função de configuração do sensor.
 * @details Esta função armazena os pinos de trigger e echo em variáveis estáticas e os configura
 *          adequadamente. O `trigPin` é configurado como uma saída (`OUTPUT`). O `echoPin` é configurado
 *          como `INPUT_PULLUP`, que ativa um resistor de pull-up interno no ESP32. Isso garante que o pino
 *          tenha um estado lógico definido (ALTO) quando não estiver recebendo o sinal de eco, tornando a
 *          leitura mais estável e menos suscetível a ruídos elétricos.
 * @param trig Pino de trigger.
 * @param echo Pino de echo.
 */
void configurarSensor(int trig, int echo) {
    trigPin = trig;
    echoPin = echo;
    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT_PULLUP);
}

/**
 * @brief Implementação da função de leitura de distância.
 * @details Esta função executa o ciclo de medição do sensor ultrassônico 5 vezes,
 *          com um intervalo de 1 segundo entre cada leitura, para obter uma medição mais estável.
 *          1.  Itera 5 vezes para coletar as amostras.
 *          2.  Em cada iteração, envia um pulso de trigger e mede a duração do pulso de eco.
 *          3.  Converte a duração em distância. Leituras inválidas (timeout) são descartadas.
 *          4.  Calcula a média das leituras válidas.
 *          5.  Aguarda 1 segundo antes da próxima leitura.
 * @return A distância média medida em cm, ou -1 se todas as leituras falharem.
 */
long lerDistancia() {
    long total_distance = 0;
    int valid_readings = 0;

    for (int i = 0; i < 5; i++) {
        digitalWrite(trigPin, LOW);
        delayMicroseconds(2);
        digitalWrite(trigPin, HIGH);
        delayMicroseconds(TRIGGER_PULSE_DURATION_US);
        digitalWrite(trigPin, LOW);

        long duration = pulseIn(echoPin, HIGH, PULSEIN_TIMEOUT_US);
        long distance = (duration == 0) ? -1 : duration / SOUND_SPEED_DIVISOR;

        if (distance != -1) {
            total_distance += distance;
            valid_readings++;
        }
        delay(1000); // Espera 1 segundo entre as leituras
    }

    if (valid_readings > 0) {
        return total_distance / valid_readings;
    } else {
        return -1; // Retorna -1 se todas as leituras falharem
    }
}