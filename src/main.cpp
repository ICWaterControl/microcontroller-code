#include "wifi_manager.h"
#include "mqtt_manager.h"
#include "log_manager.h"
#include "ultrasonic_sensor.h"

//const char* ssid = "CLARO_2G6FEFEE";
//const char* password = "386FEFEE";
const char* ssid = "Mandrade";
const char* password = "33534170";


const char* mqtt_server    = "0bbdda7fb11e4c4795c3e07e3ac1ff60.s1.eu.hivemq.cloud";
const int   mqtt_port      = 8883;
const char* mqtt_user      = "Pedro";
const char* mqtt_password  = "Luciene.456";

const int trigPin = 5;
const int echoPin = 18;

void setup() {
  Serial.begin(115200);
  delay(100);

  configurarSensor(trigPin, echoPin);
  iniciarSPIFFS();

  conectarWiFi(ssid, password);
  sincronizarHorarioNTP();

  configurarMQTT(mqtt_server, mqtt_port, mqtt_user, mqtt_password);
  conectarMQTT();

  tentarEnviarLogsPendentes();
  publicarDistancia();
}

void loop() {
  static unsigned long ultimaLeitura = 0;
  const unsigned long intervalo = 60000;
  unsigned long agora = millis();

  getMQTTClient().loop();

  if (agora - ultimaLeitura >= intervalo) {
    ultimaLeitura = agora;
    publicarDistancia();
  }
}
