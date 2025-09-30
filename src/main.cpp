#include "../include/wifi_manager.h"
#include "../include/mqtt_manager.h"
#include "../include/publish_manager.h"
#include "../include/ultrasonic_sensor.h"

//const char* ssid = "CLARO_2G6FEFEE";
//const char* password = "386FEFEE";
//const char* ssid = "Mandrade";
//const char* password = "33534170";
const char* ssid = "Wokwi-GUEST";
const char* password = "";

const char* mqtt_server    = "0bbdda7fb11e4c4795c3e07e3ac1ff60.s1.eu.hivemq.cloud";
const int   mqtt_port      = 8883;
const char* mqtt_user      = "Pedro";
const char* mqtt_password  = "Luciene.456";

const int trigPin = 5;
const int echoPin = 18;

static bool memoria_montada;

const unsigned long intervalo = 5000; //60000;

void setup() {
  Serial.begin(115200);
  delay(100);

  Serial.println("ESP32 is running");

  memoria_montada = iniciarLittleFS();
  configurarSensor(trigPin, echoPin);

  conectarWiFi(ssid, password); 
  sincronizarHorarioNTP();      

  configurarMQTT(mqtt_server, mqtt_port, mqtt_user, mqtt_password);
  conectarMQTT();            

  tentarEnviarLogsPendentes(&memoria_montada);
  publicarDistancia(&memoria_montada);
}

void loop() {
  static unsigned long ultimaLeitura = 0;
  unsigned long agora = millis();

  getMQTTClient().loop();

  if (agora - ultimaLeitura >= intervalo) {
    ultimaLeitura = agora;
    publicarDistancia(&memoria_montada);
  }
}
