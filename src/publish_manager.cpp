#include "../include/publish_manager.h"
#include "../include/mqtt_manager.h"

#include "FS.h"
#include <LittleFS.h>
#include <ArduinoJson.h>

static const char* topico_pendentes = "sistema/pendentes";

// Cria mensagem de log
String criarJsonLog(const String& mensagem, const String& nivel, const String& tipo = "sistema", int distancia = -1) {
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

// Inicia memória FLASH
bool iniciarLittleFS() {
  if (!LittleFS.begin(true)) {
    Serial.println("[LittleFS] Falha ao montar LittleFS");
    return false;
  }
  return true;
}

void publishMessage(const String& mensagem, const String& nivel, const String& tipo, const char* topico, int distancia, bool memoria_montada) {
  String logStr = criarJsonLog(mensagem, nivel, tipo, distancia);

  Serial.println(logStr); // Imprime log no monitor serial (depuração)

  auto& client = getMQTTClient(); // Obtém o cliente MQTT (de mqtt_manager.h)

  // Tentar enviar o log via MQTT
  if (client.connected() && client.publish(topico, logStr.c_str())) {
    Serial.println("[MQTT] Log enviado com sucesso.");
  } else {
    // Se falhar, salva o log no memória Flash
    if (memoria_montada) {
      File file = LittleFS.open("/log.txt", FILE_APPEND);
      if (file) {
        file.println(logStr);
        file.close();
        Serial.println("[LittleFS] Log salvo localmente.");
      } else {
        Serial.println("[LittleFS] Erro ao salvar log.");
      }
    } else {
      //memoria_montada = iniciarLittleFS();
    }
  }
}

// Tenta enviar logs salvos na memória Flash
void tentarEnviarLogsPendentes(bool* memoria_montada) {
  if (!*memoria_montada) return;

  auto& client = getMQTTClient(); 
  
  if (!LittleFS.exists("/log.txt")) return;

  File file = LittleFS.open("/log.txt", "r");
  File tempFile = LittleFS.open("/temp_log.txt", FILE_WRITE);

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
  LittleFS.remove("/log.txt");

  if (algumFalhou) {
    LittleFS.rename("/temp_log.txt", "/log.txt");
  } else {
    LittleFS.remove("/temp_log.txt");
  }
}
