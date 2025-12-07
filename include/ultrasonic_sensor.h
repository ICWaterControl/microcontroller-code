/**
 * @file ultrasonic_sensor.h
 * @brief Cabeçalho do driver e da lógica de publicação para o sensor ultrassônico.
 * @details Este arquivo define a interface para as funções que controlam o sensor ultrassônico HC-SR04,
 *          incluindo a configuração e a medição de distância.
 */

#ifndef ULTRASONIC_SENSOR_H
#define ULTRASONIC_SENSOR_H

#include <Arduino.h>

/**
 * @brief Configura os pinos do sensor ultrassônico.
 * @details Esta função inicializa os pinos de trigger e echo do sensor. Ela deve ser chamada
 *          uma vez durante a fase de setup do programa para garantir que o hardware esteja
 *          corretamente configurado antes de qualquer tentativa de leitura.
 * @param trig Pino do microcontrolador conectado ao pino 'Trig' do sensor.
 * @param echo Pino do microcontrolador conectado ao pino 'Echo' do sensor.
 */
void configurarSensor(int trig, int echo);

/**
 * @brief Realiza uma medição de distância.
 * @details Dispara o sensor ultrassônico e mede o tempo de retorno do pulso de eco para
 *          calcular a distância de um objeto. A função contém a lógica de temporização
 *          precisa para a comunicação com o sensor HC-SR04.
 * @return A distância medida em centímetros. Retorna um valor negativo (-1) se a leitura
 *         falhar (por exemplo, por timeout, indicando que nenhum objeto foi detectado no alcance).
 */
long lerDistancia();

#endif