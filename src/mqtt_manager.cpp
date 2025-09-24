#include "mqtt_manager.h"
#include <WiFiClientSecure.h>
#include "log_manager.h"

static WiFiClientSecure espClient;
static PubSubClient client(espClient);

static const char* _user;
static const char* _password;
static const char* _server;
static int _port;

void configurarMQTT(const char* server, int port, const char* user, const char* password) {
  _server = server;
  _port = port;
  _user = user;
  _password = password;

  espClient.setInsecure();
  client.setServer(_server, _port);
}

PubSubClient& getMQTTClient() {
  return client;
}

bool conectarMQTT(unsigned long timeoutMs) { // Timeout padrão 30s
  unsigned long start = millis();

  while (!client.connected()) {
    if (client.connect("ESP32Client", _user, _password)) {
      logMessage("MQTT conectado com sucesso", "SUCCESS");
      return true;
    } else {
      logMessage("Falha ao conectar no MQTT. Código: " + String(client.state()), "ERROR");
      delay(5000);
    }
    if (millis() - start > timeoutMs) {
      logMessage("Timeout ao tentar conectar MQTT", "ERROR");
      return false;
    }
  }
  return false;
}
