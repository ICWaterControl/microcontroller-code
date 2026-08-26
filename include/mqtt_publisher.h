#ifndef MQTT_PUBLISHER_H
#define MQTT_PUBLISHER_H

#include <Arduino.h>

class MqttManager;

// Publicador MQTT com fallback para SPIFFS
class MqttPublisher {
public:
    // Constrói com referência ao gerenciador MQTT
    explicit MqttPublisher(MqttManager& mqtt);

    // Publica mensagem no tópico; salva em SPIFFS se falhar
    void publicar(const char* topic, const String& jsonPayload);

    // Reenvia logs pendentes armazenados no SPIFFS
    void enviarLogsPendentes();

private:
    MqttManager& _mqtt;
};

#endif
