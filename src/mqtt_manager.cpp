/**
 * @file mqtt_manager.cpp
 * @brief Implementação da classe MqttManager.
 * @details Gerencia o cliente MQTT global, lida com a configuração, o acesso à instância
 *          do cliente e a lógica de conexão/reconexão com o broker MQTT. A comunicação é
 *          feita de forma segura usando WiFiClientSecure, e os eventos de conexão são
 *          registrados através do PublishManager.
 */

#include "../include/mqtt_manager.h"
#include "../include/publish_manager.h"
#include "../include/aws_iot_config.h"

/**
 * @brief Construtor: inicializa o PubSubClient com o WiFiClientSecure interno.
 */
MqttManager::MqttManager()
    : _client(_espClient) {}

void MqttManager::setPublishManager(PublishManager* pm) {
    _publishManager = pm;
}

/**
 * @brief Implementação da configuração do cliente MQTT.
 * @details Armazena as informações do broker e configura o TLS mútuo com certificado raiz
 *          da AWS, certificado do dispositivo e chave privada do dispositivo.
 */
void MqttManager::configurar(const char* server, int port) {
    _server = server;
    _port = port;

    _espClient.setCACert(AWS_IOT_ROOT_CA);
    _espClient.setCertificate(AWS_IOT_DEVICE_CERT);
    _espClient.setPrivateKey(AWS_IOT_PRIVATE_KEY);
    _client.setServer(_server, _port);
}

PubSubClient& MqttManager::getClient() {
    return _client;
}

/**
 * @brief Implementação da lógica de conexão com o broker MQTT.
 * @details Entra em um loop que persiste até que a conexão seja estabelecida ou o timeout
 *          seja atingido. Usa o PublishManager (se disponível) para registrar logs.
 */
bool MqttManager::conectar(unsigned long timeoutMs) {
    unsigned long start = millis();

    while (!_client.connected()) {
        if (_client.connect(AWS_IOT_CLIENT_ID)) {
            if (_publishManager) _publishManager->publicarLogSistema("MQTT conectado com sucesso", "SUCCESS");
            return true;
        } else {
            if (_publishManager) _publishManager->publicarLogSistema("Falha ao conectar no MQTT. Código: " + String(_client.state()), "ERROR");
            delay(5000);
        }
        if (millis() - start > timeoutMs) {
            if (_publishManager) _publishManager->publicarLogSistema("Timeout ao tentar conectar MQTT", "ERROR");
            return false;
        }
    }
    return false;
}