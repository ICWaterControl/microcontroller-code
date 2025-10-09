#ifndef ULTRASONIC_SENSOR_H
#define ULTRASONIC_SENSOR_H

#include <array>

void configurarSensor(int trig, int echo);
long lerDistancia();
void publicarDadosSensor(bool*);

#endif
