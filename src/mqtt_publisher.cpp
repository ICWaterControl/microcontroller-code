#include "../include/mqtt_publisher.h"
#include "../include/mqtt_manager.h"
#include "FS.h"
#include <SPIFFS.h>

// Tópico MQTT para onde os logs pendentes serão enviados.
static const char* topico_pendentes = "sistema/pendentes";

/**
 * @brief Publica uma mensagem em um tópico MQTT com fallback para SPIFFS.
 * @param topic O tópico para o qual publicar.
 * @param jsonPayload A mensagem JSON a ser enviada.
 */
void publishMqttMessage(const char* topic, const String& jsonPayload) {
  auto& client = getMQTTClient(); 

  // Tenta enviar o log via MQTT
  if (client.connected() && client.publish(topic, jsonPayload.c_str())) {
    //Serial.println("[MQTT] Log enviado com sucesso.");
  } else {
    // Se falhar, salva o log na memória Flash
    File file = SPIFFS.open("/log.txt", FILE_APPEND);
    if (file) {
      // Salva o tópico junto com o payload para reenvio posterior
      file.println(String(topic) + "|" + jsonPayload);
      file.close();
      //Serial.println("[SPIFFS] Log salvo localmente.");
    } else {
      //Serial.println("[SPIFFS] Erro ao salvar log.");
    }
  }
}

/**
 * @brief Tenta reenviar logs MQTT pendentes do SPIFFS.
 */
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
    if (linha.length() == 0) continue;

    // Separa o tópico do payload
    int separatorIndex = linha.indexOf('|');
    if (separatorIndex == -1) continue; // Formato inválido

    String topico = linha.substring(0, separatorIndex);
    String payload = linha.substring(separatorIndex + 1);

    if (client.connected()) {
      if (!client.publish(topico.c_str(), payload.c_str())) {
        tempFile.println(linha); // Salva a linha original se falhar
        algumFalhou = true;
      } else {
        //Serial.println("[MQTT] Log pendente reenviado com sucesso.");
      }
    } else {
      tempFile.println(linha); // Salva a linha original se offline
      algumFalhou = true;
      break; // Sai do loop se o cliente não está conectado
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
