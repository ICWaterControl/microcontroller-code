#ifndef BATTERY_SENSOR_H
#define BATTERY_SENSOR_H

/**
 * @brief Configura o sensor de bateria (fuel gauge).
 * @details Inicializa a comunicação I2C com o sensor MAX1704x nos pinos corretos.
 *          Deve ser chamada uma vez durante a fase de setup.
 */
void setupBatterySensor();

/**
 * @brief Lê os dados do sensor de bateria.
 * @details Obtém a porcentagem do estado de carga (SOC) e a voltagem da bateria.
 * @param percentage Referência para uma variável float onde a porcentagem será armazenada.
 * @param voltage Referência para uma variável float onde a voltagem será armazenada.
 */
void lerDadosBateria(float& percentage, float& voltage);

#endif
