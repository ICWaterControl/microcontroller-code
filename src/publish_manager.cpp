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

// Tópicos MQTT
const char* PublishManager::TOPICO_DISTANCIA = "sdk/test/java";
const char* PublishManager::TOPICO_BATERIA   = "sdk/test/python";
const char* PublishManager::TOPICO_SISTEMA   = "sdk/test/js";

PublishManager::PublishManager(MqttPublisher& publisher, UltrasonicSensor& ultrasonic, BatterySensor& battery)
    : _publisher(publisher), _ultrasonic(ultrasonic), _battery(battery) {}

/**
 * @brief Cria uma string de log formatada em JSON.
 */
String PublishManager::criarJsonLog(const String& mensagem, const String& status,
                                     int distancia, float batteryPercentage, float batteryVoltage) {
    JsonDocument doc;
    struct tm timeinfo;

    if (getLocalTime(&timeinfo)) {
        char timeString[64];
        strftime(timeString, sizeof(timeString), "%Y-%m-%d %H:%M:%S", &timeinfo);
        doc["timestamp"] = String(timeString);
    } else {
        doc["timestamp"] = "Erro ao obter horario";
    }

    // Adiciona os campos ao JSON
    doc["status"] = status;
    doc["origem"] = "esp32";
    doc["mensagem"] = mensagem;

    if (distancia >= 0) {
        doc["distancia"] = distancia;
    }
    if (batteryPercentage >= 0.0f) {
        doc["batteryPercentage"] = batteryPercentage;
    }
    if (batteryVoltage >= 0.0f) {
        doc["batteryVoltage"] = batteryVoltage;
    }

    String resultado;
    serializeJson(doc, resultado);
    return resultado;
}

/**
 * @brief Inicializa o sistema de arquivos SPIFFS.
 */
bool PublishManager::iniciarSPIFFS() {
    if (!SPIFFS.begin(true)) {
        //Serial.println("[SPIFFS] Falha ao montar SPIFFS");
        return false;
    }
    return true;
}

/**
 * @brief Orquestra a publicação dos dados de distância.
 */
void PublishManager::publicarDistancia() {
    long distancia = _ultrasonic.lerDistancia();

    String jsonPayload;
    if (distancia < 0) {
        jsonPayload = criarJsonLog("Erro na leitura do sensor", "ERROR");
    } else {
        String msg = "Distancia lida: " + String(distancia);
        jsonPayload = criarJsonLog(msg, "SUCCESS", distancia);
    }

    //Serial.println(jsonPayload);
    _publisher.publicar(TOPICO_DISTANCIA, jsonPayload);
}

/**
 * @brief Orquestra a publicação dos dados da bateria.
 */
void PublishManager::publicarBateria() {
    float batteryPercentage, batteryVoltage;
    _battery.lerDados(batteryPercentage, batteryVoltage);

    String msg = "Bateria: " + String(batteryPercentage) + "% (" + String(batteryVoltage) + "V)";
    String jsonPayload = criarJsonLog(msg, "SUCCESS", -1, batteryPercentage, batteryVoltage);

    //Serial.println(jsonPayload);
    _publisher.publicar(TOPICO_BATERIA, jsonPayload);
}

/**
 * @brief Publica uma mensagem de log genérica do sistema.
 */
void PublishManager::publicarLogSistema(const String& mensagem, const String& status) {
    String jsonPayload = criarJsonLog(mensagem, status);

    //Serial.println(jsonPayload);
    _publisher.publicar(TOPICO_SISTEMA, jsonPayload);
}