#ifndef MQTT_PUBLISHER_H
#define MQTT_PUBLISHER_H

#include <Arduino.h>

/**
 * @brief Publica uma mensagem em um tópico MQTT.
 * @details Tenta publicar a mensagem no broker MQTT. Se a publicação falhar
 *          (por exemplo, por falta de conexão), a mensagem é salva em um arquivo
 *          de log no sistema de arquivos SPIFFS para envio posterior.
 * 
 * @param topic O tópico MQTT para o qual a mensagem será publicada.
 * @param jsonPayload A string JSON a ser enviada.
 */
void publishMqttMessage(const char* topic, const String& jsonPayload);

/**
 * @brief Tenta reenviar logs MQTT salvos localmente.
 * @details Verifica se há logs pendentes no arquivo de log do SPIFFS e tenta
 *          reenviar cada um para o broker MQTT. Os logs que são enviados com
 *          sucesso são removidos do arquivo.
 */
void tentarEnviarLogsPendentes();

#endif
