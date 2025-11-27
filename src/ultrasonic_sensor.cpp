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
 * @details Esta função executa o ciclo de medição do sensor ultrassônico:
 *          1. Garante que o pino de trigger esteja em nível baixo.
 *          2. Envia um pulso de disparo (nível alto) com a duração definida por `TRIGGER_PULSE_DURATION_US`.
 *          3. Retorna o pino de trigger para o nível baixo.
 *          4. Usa a função `pulseIn` para medir o tempo que o pino de eco permanece em nível alto, com um timeout
 *             definido por `PULSEIN_TIMEOUT_US`.
 *          5. Se `pulseIn` retornar 0 (timeout), a função retorna -1. Caso contrário, a duração é dividida
 *             pelo `SOUND_SPEED_DIVISOR` para obter a distância em centímetros.
 * @return A distância medida em cm, ou -1 em caso de falha na leitura (timeout).
 */
long lerDistancia() {
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(TRIGGER_PULSE_DURATION_US);
    digitalWrite(trigPin, LOW);

    long duration = pulseIn(echoPin, HIGH, PULSEIN_TIMEOUT_US);
    return (duration == 0) ? -1 : duration / SOUND_SPEED_DIVISOR;
}