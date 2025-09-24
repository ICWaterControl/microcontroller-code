#include "log_manager.h"
#include "FS.h"
#include "SPIFFS.h"
#include <ArduinoJson.h>
#include "mqtt_manager.h"

static const char* mqtt_logs = "meutopico/logs";

String criarJsonLog(const String& mensagem, const String& nivel, const String& tipo = "sistema", int distancia = -1) {
  DynamicJsonDocument doc(512);
  struct tm timeinfo;

  if (getLocalTime(&timeinfo)) {
    char timeString[64];
    strftime(timeString, sizeof(timeString), "%Y-%m-%d %H:%M:%S", &timeinfo);
    doc["timestamp"] = String(timeString);
  } else {
    doc["timestamp"] = "Erro ao obter horario";
  }

  doc["nivel"] = nivel;
  doc["origem"] = "esp32";
  doc["tipo"] = tipo;
  doc["mensagem"] = mensagem;

  if (distancia >= 0) {
    doc["distancia"] = distancia;
  }

  String resultado;
  serializeJson(doc, resultado);
  return resultado;
}

void iniciarSPIFFS() {
  if (!SPIFFS.begin(true)) {
    Serial.println("[SPIFFS] Falha ao montar SPIFFS");
  }
}

void logMessage(const String& mensagem, const String& nivel, const String& tipo, int distancia) {
  String logStr = criarJsonLog(mensagem, nivel, tipo, distancia);
  Serial.println(logStr);

  auto& client = getMQTTClient();
  if (client.connected() && client.publish(mqtt_logs, logStr.c_str())) {
    Serial.println("[MQTT] Log enviado com sucesso.");
  } else {
    File file = SPIFFS.open("/log.txt", FILE_APPEND);
    if (file) {
      file.println(logStr);
      file.close();
      Serial.println("[SPIFFS] Log salvo localmente.");
    } else {
      Serial.println("[SPIFFS] Erro ao salvar log.");
    }
  }
}

void tentarEnviarLogsPendentes() {
  auto& client = getMQTTClient();
  if (!SPIFFS.exists("/log.txt")) return;

  File file = SPIFFS.open("/log.txt", "r");
  File tempFile = SPIFFS.open("/temp_log.txt", FILE_WRITE);

  if (!file || !tempFile) {
    if (file) file.close();
    return;
  }

  bool algumFalhou = false;
  while (file.available()) {
    String linha = file.readStringUntil('\n');
    linha.trim();
    if (linha.length() < 10) continue;

    if (client.connected()) {
      if (!client.publish(mqtt_logs, linha.c_str())) {
        tempFile.println(linha);
        algumFalhou = true;
      }
    } else {
      tempFile.println(linha);
      algumFalhou = true;
    }
  }

  file.close();
  tempFile.close();
  SPIFFS.remove("/log.txt");

  if (algumFalhou) {
    SPIFFS.rename("/temp_log.txt", "/log.txt");
  } else {
    SPIFFS.remove("/temp_log.txt");
  }
}
