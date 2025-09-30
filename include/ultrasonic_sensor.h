#ifndef ULTRASONIC_SENSOR_H
#define ULTRASONIC_SENSOR_H

void configurarSensor(int trig, int echo);
long lerDistancia();
void publicarDistancia(bool*);

#endif
