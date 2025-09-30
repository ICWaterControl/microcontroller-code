#include "../include/ultrasonic_sensor.h"
#include "../include/publish_manager.h"
#include "../include/mqtt_manager.h"

static int trigPin, echoPin;

static const char* topico_distancia = "sensor/distancia";

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

  //publishMessage("Distancia lida: " + mensagem, "INFO", topico_distancia, distancia, *memoria_montada);

  if (getMQTTClient().connected()) {
    publishMessage("Distancia lida publicada com sucesso via MQTT", "SUCCESS", topico_distancia, distancia, *memoria_montada);
  } else {
    publishMessage("Falha ao publicar distancia via MQTT", "ERROR", topico_distancia, distancia, memoria_montada);
  }
  
  tentarEnviarLogsPendentes(memoria_montada);
}
