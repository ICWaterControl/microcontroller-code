/**
 * @file ultrasonic_sensor.h
 * @brief Cabeçalho para o driver do sensor ultrassônico HC-SR04.
 * @details Este arquivo define a interface para o gerenciamento do sensor ultrassônico. Ele é responsável
 *          por abstrair a complexidade do hardware, fornecendo funções simples para configurar o sensor,
 *          realizar leituras de distância e publicar os dados obtidos.
 */
#ifndef ULTRASONIC_SENSOR_H
#define ULTRASONIC_SENSOR_H

#include <array>

/**
 * @brief Configura os pinos do sensor ultrassônico.
 * @details Esta função deve ser chamada uma vez durante a inicialização do sistema (na função `setup()`)
 *          para definir quais pinos do microcontrolador estão conectados aos pinos TRIG (disparo) e ECHO (eco)
 *          do sensor HC-SR04. A função configura o pino de trigger como uma saída digital e o pino de echo
 *          como uma entrada digital.
 * @param trig O número do pino digital do ESP32 conectado ao pino TRIG do sensor.
 * @param echo O número do pino digital do ESP32 conectado ao pino ECHO do sensor.
 */
void configurarSensor(int trig, int echo);

/**
 * @brief Realiza uma medição de distância com o sensor ultrassônico.
 * @details Esta função implementa o processo de medição de distância. Ela funciona da seguinte maneira:
 *          1. Envia um pulso de disparo de 10 microssegundos no pino TRIG.
 *          2. Aguarda o pino ECHO ir para o nível alto, indicando o recebimento do eco.
 *          3. Mede a duração do tempo em que o pino ECHO permanece em nível alto.
 *          4. Converte essa duração (em microssegundos) para centímetros usando a velocidade do som.
 *          A função inclui um timeout para evitar que o programa fique bloqueado caso o eco não seja detectado.
 * @return A distância medida em centímetros. Retorna -1 se a leitura exceder o tempo limite (timeout),
 *         indicando uma falha na medição.
 */
long lerDistancia();

/**
 * @brief Lê a distância e publica os dados do sensor.
 * @details Esta é uma função de alto nível que orquestra a leitura e a publicação dos dados. Ela chama
 *          `lerDistancia()` para obter a medição atual e, em seguida, formata e envia esses dados como um log.
 *          A função também lida com a lógica de registrar o sucesso ou a falha da publicação dos dados via MQTT.
 *          Além disso, ela tenta reenviar logs que possam estar pendentes de envios anteriores.
 * @param mudancaDetectada Ponteiro para uma variável booleana. A função definirá este valor como `true` se a
 *                         distância medida for diferente da anterior e estiver dentro de um limite válido,
 *                         caso contrário, definirá como `false`.
 */
void publicarDadosSensor(bool* mudancaDetectada);

#endif