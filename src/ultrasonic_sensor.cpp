#include "ultrasonic_sensor.h"
#include "log_manager.h"
#include "mqtt_manager.h"

static int trigPin, echoPin;

void configurarSensor(int trig, int echo) {
  trigPin = trig;
  echoPin = echo;
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

long lerDistancia() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH, 30000);
  return (duration == 0) ? -1 : duration / 58;
}

void publicarDistancia() {
  long distancia = lerDistancia();
  String mensagem = (distancia >= 0) ? String(distancia) : "Erro na leitura do sensor";

  logMessage("Distancia lida: " + mensagem, "INFO", "sensor", distancia);

  if (getMQTTClient().connected()) {
    logMessage("Distancia publicada com sucesso via MQTT", "SUCCESS", "sensor");
  } else {
    logMessage("Falha ao publicar distancia via MQTT", "ERROR", "sensor");
  }

  tentarEnviarLogsPendentes();
}
