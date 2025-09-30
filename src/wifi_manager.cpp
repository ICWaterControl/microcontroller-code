#include <WiFi.h>
#include <time.h>

#include "../include/wifi_manager.h"
#include "../include/publish_manager.h"

static const char* topico = "sistema/comunicacao/wifi";
static const String& tipo = "WI-FI";

void conectarWiFi(const char* ssid, const char* password) {
  WiFi.begin(ssid, password);
  Serial.print("[WiFi] Conectando");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  publishMessage("Wi-Fi conectado com sucesso", "SUCCESS", tipo, topico);
}

bool sincronizarHorarioNTP() {
  configTime(-3 * 3600, 0, "pool.ntp.org", "time.nist.gov");
  struct tm timeinfo;
  for (int i = 0; i < 10; i++) {
    if (getLocalTime(&timeinfo)) {
      publishMessage("Tempo NTP sincronizado com sucesso", "SUCCESS", tipo, topico);
      return true;
    }
    delay(1000);
  }
  publishMessage("Falha ao obter tempo via NTP", "ERROR", tipo, topico);
  return false;
}
