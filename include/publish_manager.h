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
 * @brief Publica ou registra uma mensagem.
 * @details Esta função é o núcleo do sistema de logging. Ela constrói uma mensagem em formato JSON contendo
 *          timestamp, nível de log, origem, a mensagem em si e, opcionalmente, dados de um sensor (distância).
 *          A função então tenta publicar essa mensagem em um tópico MQTT. Se a publicação falhar (por exemplo,
 *          por falta de conexão), a mensagem é salva em um arquivo de log no sistema de arquivos SPIFFS para
 *          envio posterior. A função também imprime o log na porta serial para depuração.
 * @param mensagem A string da mensagem principal a ser registrada.
 * @param status O nível ou status do log (ex: "INFO", "ERROR", "SUCCESS"). O padrão é "INFO".
 * @param topico O tópico MQTT específico para onde a mensagem deve ser publicada. Se vazio, um tópico padrão será usado.
 * @param distancia Um valor numérico opcional, como a distância de um sensor, a ser incluído no log. O padrão é -1 (não incluído).
 * @param batteryPercentage A porcentagem da bateria. O padrão é -1.0 (não incluído).
 * @param batteryVoltage A voltagem da bateria. O padrão é -1.0 (não incluído).
 */
void publishMessage(const String& mensagem, const String& status = "INFO", const char* topico = "", int distancia = -1, float batteryPercentage = -1.0, float batteryVoltage = -1.0);

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
 * @brief Tenta reenviar logs salvos localmente.
 * @details Esta função verifica se há logs pendentes no arquivo de log do SPIFFS. Se houver, ela tenta
 *          reenviar cada log para o broker MQTT. Os logs que são enviados com sucesso são removidos do arquivo,
 *          enquanto os que falham permanecem para tentativas futuras. Isso garante que os logs não sejam perdidos
 *          se o dispositivo estiver offline. A operação é feita de forma atômica usando um arquivo temporário
 *          para evitar a perda de dados em caso de reinicialização.
 */
void tentarEnviarLogsPendentes();

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