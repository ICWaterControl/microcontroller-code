#pragma once

#include <Arduino.h>

void publishMessage(const String& mensagem, const String& status = "INFO", const char* topico = "", int distancia = -1, bool = false);
void tentarEnviarLogsPendentes(bool*);
bool iniciarSPIFFS();