#include "wifi_manager.h"
#include <WiFi.h>
#include <time.h>
#include "log_manager.h"

void conectarWiFi(const char* ssid, const char* password) {
  WiFi.begin(ssid, password);
  Serial.print("[WiFi] Conectando");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  logMessage("Wi-Fi conectado com sucesso", "SUCCESS");
}

bool sincronizarHorarioNTP() {
  configTime(-3 * 3600, 0, "pool.ntp.org", "time.nist.gov");
  struct tm timeinfo;
  for (int i = 0; i < 10; i++) {
    if (getLocalTime(&timeinfo)) {
      logMessage("Tempo NTP sincronizado com sucesso", "SUCCESS");
      return true;
    }
    delay(1000);
  }
  logMessage("Falha ao obter tempo via NTP", "ERROR");
  return false;
}
