/**
 * @file publish_manager.cpp
 * @brief Implementação da classe PublishManager.
 * @details Contém a lógica de criação de mensagens de log em formato JSON, a inicialização
 *          do SPIFFS e a publicação de mensagens via MqttPublisher. Coordena os sensores
 *          de distância e bateria para coletar e publicar os dados.
 */

#include "../include/publish_manager.h"
#include "../include/mqtt_publisher.h"
#include "../include/ultrasonic_sensor.h"
#include "../include/battery_sensor.h"

#include "../include/device_id_config.hpp"

// Tópicos MQTT
const String PublishManager::TOPICO_MEDICOES = "/measurement";
const String PublishManager::TOPICO_SISTEMA = "/system";

PublishManager::PublishManager(MqttPublisher &publisher, UltrasonicSensor &ultrasonic, BatterySensor &battery)
    : m_publisher(publisher), m_ultrasonic(ultrasonic), m_battery(battery) {}

String PublishManager::getTopicoMedicoes()
{
    return device_uuid + TOPICO_MEDICOES;
}
/**
 * @brief Cria uma string de log formatada em JSON.
 */
String PublishManager::criarJsonLog(const String &mensagem, const String &status,
                                    int distancia, float batteryPercentage, float batteryVoltage)
{
    JsonDocument doc;
    struct tm timeinfo;

    configTime(-3 * 3600, 0, "pool.ntp.org", "time.nist.gov");
    if (getLocalTime(&timeinfo))
    {
        char timeString[64];
        strftime(timeString, sizeof(timeString), "%Y-%m-%d %H:%M:%S", &timeinfo);
        doc["timestamp"] = String(timeString);
    }
    else
    {
        Serial.println("Erro ao obter horário dessa leitura.");
        doc["timestamp"] = "";
    }

    // Adiciona os campos ao JSON
    doc["id"] = device_uuid;
    doc["status"] = status;
    doc["origin"] = device_name;
    doc["log"] = mensagem;

    if (distancia >= 0)
    {
        doc["water-distance-cm"] = distancia;
    }
    if (batteryPercentage >= 0.0f)
    {
        doc["battery-percentage"] = batteryPercentage;
    }
    if (batteryVoltage >= 0.0f)
    {
        doc["battery-voltage"] = batteryVoltage;
    }

    String resultado;
    serializeJson(doc, resultado);
    return resultado;
}

/**
 * @brief Inicializa o sistema de arquivos SPIFFS.
 */
bool PublishManager::iniciarSPIFFS()
{
    if (!SPIFFS.begin(true))
    {
        // Serial.println("[SPIFFS] Falha ao montar SPIFFS");
        return false;
    }
    return true;
}

/**
 * @brief Orquestra a publicação dos dados de distância.
 */
void PublishManager::publicarDistancia()
{
    long distancia = m_ultrasonic.lerDistancia();

    String jsonPayload;
    if (distancia < 0)
    {
        jsonPayload = criarJsonLog("Erro na leitura do sensor", "ERROR");
    }
    else
    {
        String msg = "Leitura realizada com sucesso!";
        jsonPayload = criarJsonLog(msg, "SUCCESS", distancia);
    }

    Serial.println(jsonPayload);
    m_publisher.publicar(getTopicoMedicoes().c_str(), jsonPayload);
}

/**
 * @brief Orquestra a publicação dos dados da bateria.
 */
void PublishManager::publicarBateria()
{
    float batteryPercentage, batteryVoltage;
    m_battery.lerDados(batteryPercentage, batteryVoltage);

    String msg = "Bateria: " + String(batteryPercentage) + "% (" + String(batteryVoltage) + "V)";
    String jsonPayload = criarJsonLog(msg, "SUCCESS", -1, batteryPercentage, batteryVoltage);

    // Serial.println(jsonPayload);
    m_publisher.publicar(getTopicoMedicoes().c_str(), jsonPayload);
}

/**
 * @brief Publica uma mensagem de log genérica do sistema.
 */
void PublishManager::publicarLogSistema(const String &mensagem, const String &status)
{
    String jsonPayload = criarJsonLog(mensagem, status);

    // Serial.println(jsonPayload);
    // m_publisher.publicar(TOPICO_SISTEMA, jsonPayload);
}