/**
 * @file mqtt_manager.h
 * @brief Cabeçalho do gerenciador de conexão MQTT.
 * @details Este arquivo define a interface(Contrato) para o gerenciamento da comunicação com um broker MQTT.
 *          Ele abstrai a complexidade da biblioteca PubSubClient, fornecendo um conjunto de funções
 *          simplificadas para configurar, conectar e interagir com o cliente MQTT. O objetivo é
 *          centralizar toda a lógica de MQTT, facilitando a manutenção e o uso em outras partes do sistema.
 */
#ifndef MQTT_MANAGER_H
#define MQTT_MANAGER_H

#include <PubSubClient.h>
#include <WiFiClientSecure.h>

/**
 * @brief Configura os parâmetros de conexão do cliente MQTT.
 * @details Esta função deve ser chamada uma vez na inicialização do sistema, antes da primeira tentativa de conexão.
 *          Ela armazena as informações do broker e configura o cliente MQTT subjacente (PubSubClient) para usar
 *          TLS mútuo com certificado raiz, certificado do dispositivo e chave privada.
 * @param server O endereço hostname do broker MQTT.
 * @param port A porta de conexão do broker (8883 para TLS).
 */
void configurarMQTT(const char* server, int port);

/**
 * @brief Obtém uma referência para a instância global do cliente MQTT.
 * @details Esta função fornece acesso direto à instância do objeto PubSubClient. Isso é útil quando
 *          outras partes do código precisam de mais controle sobre o cliente, como para se inscrever em tópicos
 *          (subscribe) ou para chamar a função `loop()` que mantém a conexão ativa e processa as mensagens
 *          recebidas. O uso de uma referência (`&`) evita a cópia do objeto, garantindo que todos os módulos
 *          estejam trabalhando com a mesma e única instância do cliente.
 * @return Uma referência (`PubSubClient&`) para o objeto do cliente MQTT.
 */
PubSubClient& getMQTTClient();

/**
 * @brief Tenta estabelecer uma conexão com o broker MQTT.
 * @details Esta função encapsula a lógica de conexão e reconexão. Ela entra em um laço e tenta se conectar
 *          ao broker usando as credenciais previamente definidas em `configurarMQTT`. Se a conexão for bem-sucedida,
 *          a função retorna `true`. Se falhar, ela aguarda um intervalo de 5 segundos e tenta novamente.
 *          Todo o processo tem um tempo limite (timeout) para evitar que o programa fique bloqueado indefinidamente
 *          em caso de falha persistente na rede ou no broker. Logs são gerados para cada tentativa, sucesso ou falha.
 * @param timeoutMs O tempo máximo, em milissegundos, que a função deve continuar tentando se conectar antes de desistir.
 *                  O valor padrão é 30000 ms (30 segundos).
 * @return `true` se a conexão for estabelecida com sucesso dentro do tempo limite, `false` caso contrário.
 */
bool conectarMQTT(unsigned long timeoutMs = 30000);

#endif