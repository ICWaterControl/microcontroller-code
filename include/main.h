
/**
 * @file main.h
 * @brief Cabeçalho principal que agrega os módulos do sistema.
 * @details Este arquivo centraliza as inclusões de módulos usados pelo arquivo `main.cpp`.
 *          Em projetos embarcados é comum ter um cabeçalho "agregador" que reune as interfaces
 *          de rede, publicação, sensores e outras funcionalidades para uso no ponto de entrada.
 *          Não deve conter definições de funções ou variáveis globais com linkage externo;
 *          apenas inclusões e declarações (forward declarations) necessárias ao `main.cpp`.
 */
#ifndef MAIN_H
#define MAIN_H

#include "../include/wifi_manager.h"
#include "../include/mqtt_manager.h"
#include "../include/publish_manager.h"
#include "../include/ultrasonic_sensor.h"

#include <WiFi.h>
#include <cstdio>

#endif // MAIN_H