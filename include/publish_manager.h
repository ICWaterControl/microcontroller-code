#pragma once

#include <Arduino.h>

void publishMessage(const String& mensagem, const String& status = "INFO", const char* topico = "", int distancia = -1);
void tentarEnviarLogsPendentes();
bool iniciarSPIFFS();