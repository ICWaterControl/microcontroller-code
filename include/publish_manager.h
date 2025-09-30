#pragma once

#include <Arduino.h>

void publishMessage(const String& mensagem, const String& nivel = "INFO", const String& tipo = "sistema", const char* topico = "", int distancia = -1, bool = false);
void tentarEnviarLogsPendentes(bool*);
bool iniciarLittleFS();