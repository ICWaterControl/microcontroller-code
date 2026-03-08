/**
 * @file publish_manager.cpp
 * @brief Implementação do gerenciador de publicação e logging.
 * @details Este arquivo contém a lógica de implementação para o sistema de logging e publicação. Ele lida com a
 *          criação de mensagens de log em formato JSON, a inicialização do sistema de arquivos SPIFFS, a publicação
 *          de mensagens via MQTT e o armazenamento local de logs quando a rede está indisponível.
 */

#include "../include/publish_manager.h"
#include "../include/mqtt_manager.h"
#include "../include/mqtt_publisher.h"
#include "../include/google_sheets_publisher.h"
#include "../include/ultrasonic_sensor.h"
#include "../include/battery_sensor.h"

// Tópicos MQTT
static const char* topico_distancia = "sensor/distancia";
static const char* topico_bateria = "sensor/bateria";
static const char* topico_sistema = "sistema/log";

// Protótipos de Funções Internas
String criarJsonLog(const String& mensagem, const String& status, int distancia = -1, float batteryPercentage = -1.0, float batteryVoltage = -1.0);


/**
 * @brief Cria uma string de log formatada em JSON.
 */
String criarJsonLog(const String& mensagem, const String& status, int distancia, float batteryPercentage, float batteryVoltage) {
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
  if (batteryPercentage >= 0.0) {
    doc["batteryPercentage"] = batteryPercentage;
  }
  if (batteryVoltage >= 0.0) {
    doc["batteryVoltage"] = batteryVoltage;
  }

  String resultado;
  serializeJson(doc, resultado);
  return resultado;
}

/**
 * @brief Inicializa o sistema de arquivos SPIFFS.
 */
bool iniciarSPIFFS() {
  if (!SPIFFS.begin(true)) {
    Serial.println("[SPIFFS] Falha ao montar SPIFFS");
    return false;
  }
  return true;
}

/**
 * @brief Orquestra a publicação dos dados de distância.
 */
void publicarLeituraDistancia(bool& conectado) {
    long distancia = lerDistancia();

    String jsonPayload;
    if (distancia < 0) {
        jsonPayload = criarJsonLog("Erro na leitura do sensor", "ERROR");
    } else {
        String msg = "Distancia lida: " + String(distancia);
        jsonPayload = criarJsonLog(msg, "SUCCESS", distancia);
    }
    
    Serial.println(jsonPayload);
    publishToGoogleSheets(jsonPayload);
    publishMqttMessage(topico_distancia, jsonPayload);
    
    // A verificação de conexão fica centralizada no loop principal
    if (!getMQTTClient().connected()) {
        conectado = false;
    }
}

/**
 * @brief Orquestra a publicação dos dados da bateria.
 */
void publicarLeituraBateria(bool& conectado) {
    float batteryPercentage, batteryVoltage;
    lerDadosBateria(batteryPercentage, batteryVoltage);
    
    String msg = "Bateria: " + String(batteryPercentage) + "% (" + String(batteryVoltage) + "V)";
    String jsonPayload = criarJsonLog(msg, "SUCCESS", -1, batteryPercentage, batteryVoltage);

    Serial.println(jsonPayload);
    publishToGoogleSheets(jsonPayload);
    publishMqttMessage(topico_bateria, jsonPayload);

    if (!getMQTTClient().connected()) {
        conectado = false;
    }
}

/**
 * @brief Publica uma mensagem de log genérica do sistema.
 */
void publicarLogSistema(const String& mensagem, const String& status) {
    String jsonPayload = criarJsonLog(mensagem, status);

    Serial.println(jsonPayload);
    //publishToGoogleSheets(jsonPayload);
    publishMqttMessage(topico_sistema, jsonPayload);
}