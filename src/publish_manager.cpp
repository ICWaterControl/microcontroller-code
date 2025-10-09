#include "../include/publish_manager.h"
#include "../include/mqtt_manager.h"

#include "FS.h"
#include <SPIFFS.h>
#include <ArduinoJson.h>

static const char* topico_pendentes = "sistema/pendentes";

// Cria mensagem de log
String criarJsonLog(const String& mensagem, const String& status, int distancia = -1) {
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

  String resultado;
  serializeJson(doc, resultado);
  return resultado;
}

// Inicia memória FLASH
bool iniciarSPIFFS() {
  if (!SPIFFS.begin(true)) {
    Serial.println("[SPIFFS] Falha ao montar SPIFFS");
    return false;
  }
  return true;
}

void publishMessage(const String& mensagem, const String& status, const char* topico, int distancia) {
  String logStr = criarJsonLog(mensagem, status, distancia);

  Serial.println(logStr); // Imprime log no monitor serial (depuração)

  auto& client = getMQTTClient(); 

  // Tentar enviar o log via MQTT
  if (client.connected() && client.publish(topico, logStr.c_str())) {
    Serial.println("[MQTT] Log enviado com sucesso.");
  } else {
    // Se falhar, salva o log no memória Flash
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

// Tenta enviar logs salvos na memória Flash
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
      if (!client.publish(topico_pendentes, linha.c_str())) {
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
