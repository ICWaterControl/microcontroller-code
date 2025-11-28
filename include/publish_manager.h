/**
 * @file publish_manager.h
 * @brief Cabeçalho do gerenciador de publicação e logging.
 * @details Este arquivo define a interface para um sistema de logging e publicação de mensagens. Ele é projetado
 *          para sistemas embarcados, como o ESP32, que podem precisar armazenar mensagens localmente quando a
 *          conectividade de rede não está disponível. As funções aqui declaradas permitem publicar mensagens
 *          para um tópico MQTT, registrar logs com diferentes níveis de status e gerenciar um sistema de arquivos
 *          (SPIFFS) para persistência de logs.
 */
// Diretiva de pré-processador que garante que este arquivo de cabeçalho seja
// incluído apenas uma vez por unidade de compilação, evitando erros de redefinição.
#ifndef PUBLISH_MANAGER_H
#define PUBLISH_MANAGER_H

#include <Arduino.h>
#include "FS.h"
#include <SPIFFS.h>
#include <ArduinoJson.h>

/**
 * @brief Orquestra a leitura e publicação dos dados do sensor de distância.
 * @details Esta função lê a distância do sensor ultrassônico e publica o resultado em um
 *          tópico MQTT específico. Se a conexão MQTT não estiver ativa, os dados são
 *          salvos localmente para envio posterior.
 * @param conectado Ponteiro para a flag de estado da conexão Wi-Fi. A função pode
 *                  modificar este valor para `false` se a publicação MQTT falhar.
 */
void publicarLeituraDistancia(bool* conectado);

/**
 * @brief Orquestra a leitura e publicação dos dados da bateria.
 * @details Esta função lê a voltagem e a porcentagem da bateria e publica os resultados em um
 *          tópico MQTT específico. Se a conexão MQTT não estiver ativa, os dados são
 *          salvos localmente para envio posterior.
 * @param conectado Ponteiro para a flag de estado da conexão Wi-Fi. A função pode
 *                  modificar este valor para `false` se a publicação MQTT falhar.
 */
void publicarLeituraBateria(bool* conectado);

/**
 * @brief Publica uma mensagem de log genérica do sistema.
 * @details Usada para publicar eventos de status do sistema (ex: conexão WiFi, MQTT).
 *          Cria um payload JSON e o envia para todos os publicadores configurados.
 * @param mensagem A mensagem de log a ser enviada.
 * @param status O status da mensagem (ex: "SUCCESS", "ERROR").
 */
void publicarLogSistema(const String& mensagem, const String& status);


/**
 * @brief Inicializa o sistema de arquivos SPIFFS.
 * @details O SPIFFS (SPI Flash File System) é usado para armazenar logs quando o dispositivo está offline.
 *          Esta função monta o sistema de arquivos. Se a montagem falhar, uma mensagem de erro será
 *          impressa no monitor serial. É crucial que esta função seja chamada com sucesso durante a
 *          inicialização para que a persistência de logs funcione.
 * @return `true` se o SPIFFS foi iniciado com sucesso, `false` caso contrário.
 */
bool iniciarSPIFFS();

#endif