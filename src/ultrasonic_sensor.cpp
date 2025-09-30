#include "../include/ultrasonic_sensor.h"
#include "../include/publish_manager.h"
#include "../include/mqtt_manager.h"

static int trigPin, echoPin;

static const char* topico_distancia = "sensor/distancia";
static const String& tipo = "DISTANCIA";

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

void publicarDistancia(bool *memoria_montada) {
  long distancia = lerDistancia();
  String mensagem = (distancia >= 0) ? String(distancia) : "Erro na leitura do sensor";

  publishMessage("Distancia lida: " + mensagem, "INFO", tipo, topico_distancia, distancia, *memoria_montada);

  if (getMQTTClient().connected()) {
    publishMessage("Distancia publicada com sucesso via MQTT", "SUCCESS", tipo, topico_distancia);
  } else {
    publishMessage("Falha ao publicar distancia via MQTT", "ERROR", tipo, topico_distancia);
  }
  
  tentarEnviarLogsPendentes(memoria_montada);
}
