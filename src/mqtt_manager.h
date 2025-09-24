#ifndef MQTT_MANAGER_H
#define MQTT_MANAGER_H

#include <PubSubClient.h>

void configurarMQTT(const char* server, int port, const char* user, const char* password);
PubSubClient& getMQTTClient();
bool conectarMQTT(unsigned long timeoutMs = 30000);

#endif
