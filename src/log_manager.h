#ifndef LOG_MANAGER_H
#define LOG_MANAGER_H

#include <Arduino.h>

void logMessage(const String& mensagem, const String& nivel = "INFO", const String& tipo = "sistema", int distancia = -1);
void tentarEnviarLogsPendentes();
void iniciarSPIFFS();

#endif
