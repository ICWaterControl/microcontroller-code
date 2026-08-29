#ifndef PUBLISH_MANAGER_H
#define PUBLISH_MANAGER_H

#include <Arduino.h>
#include "FS.h"
#include <SPIFFS.h>
#include <ArduinoJson.h>

class MqttPublisher;
class UltrasonicSensor;
class BatterySensor;

// Gerenciador de publicação de dados e logs do sistema
class PublishManager
{
public:
    // Constrói com referências ao publicador e sensores
    PublishManager(MqttPublisher &publisher, UltrasonicSensor &ultrasonic, BatterySensor &battery);

    // Inicializa o sistema de arquivos SPIFFS
    bool iniciarSPIFFS();

    // Lê distância do sensor e publica via MQTT
    void publicarDistancia();

    // Lê dados de bateria e publica via MQTT
    void publicarBateria();

    // Publica log genérico do sistema (ex: WiFi, MQTT)
    void publicarLogSistema(const String &mensagem, const String &status);

private:
    MqttPublisher &m_publisher;
    UltrasonicSensor &m_ultrasonic;
    BatterySensor &m_battery;

    static const String TOPICO_MEDICOES;
    static const String TOPICO_SISTEMA;

    // Retorna um tópico único baseado no uuid do dispositivo
    String getTopicoMedicoes();

    // Cria payload JSON com timestamp, status e dados opcionais
    String criarJsonLog(const String &mensagem, const String &status,
                        int distancia = -1, float batteryPercentage = -1.0f,
                        float batteryVoltage = -1.0f);
};

#endif