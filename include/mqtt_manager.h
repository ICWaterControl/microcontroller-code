#ifndef MQTT_MANAGER_H
#define MQTT_MANAGER_H

#include <PubSubClient.h>
#include <WiFiClientSecure.h>

class PublishManager;

// Gerenciador de conexão MQTT com TLS mútuo
class MqttManager {
public:
    MqttManager();

    // Injeta o PublishManager para logging (resolve dependência circular)
    void setPublishManager(PublishManager* pm);

    // Configura broker, porta e certificados TLS
    void configurar(const char* server, int port);

    // Tenta conectar ao broker dentro do timeout (ms)
    bool conectar(unsigned long timeoutMs = 30000);

    // Retorna referência ao PubSubClient interno
    PubSubClient& getClient();

private:
    WiFiClientSecure m_espClient;
    PubSubClient m_client;
    const char* m_server = nullptr;
    int m_port = 0;
    PublishManager* m_publishManager = nullptr;
};

#endif