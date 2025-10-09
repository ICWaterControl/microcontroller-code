#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

void conectarWiFi(const char* ssid, const char* password, bool* conectado);
void reconectarWiFi(const char* ssid, const char* password, bool* conectado);
bool sincronizarHorarioNTP();

#endif