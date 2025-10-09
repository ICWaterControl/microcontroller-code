#include "../include/wifi_manager.h"
#include "../include/mqtt_manager.h"
#include "../include/publish_manager.h"
#include "../include/ultrasonic_sensor.h"

#include <WiFi.h>

//const char* ssid = "CLARO_2G6FEFEE";
//const char* password = "386FEFEE";

const char* ssid = "Mandrade";
const char* password = "33534170";

//const char* ssid = "GLOBAL_FIBRA";
//const char* password = "is@88302713";

//const char* ssid = "Wokwi-GUEST";
//const char* password = "";

//const char* ssid = "Malware";
//const char* password = "isabella";

const char* mqtt_server    = "0bbdda7fb11e4c4795c3e07e3ac1ff60.s1.eu.hivemq.cloud";
const int   mqtt_port      = 8883;
const char* mqtt_user      = "Pedro";
const char* mqtt_password  = "Luciene.456";

const int trigPin = 5; // JUMPER AZUL QUE SAI DO SENSOR
const int echoPin = 18; // JUMPER AMARELO QUE SAI DO RESISTOR 

static bool conectado;

const unsigned long intervalo = 5000; //60000;

void setup() {
  Serial.begin(115200);
  delay(500);

  Serial.println("ESP32 is running");

  iniciarSPIFFS();
  configurarSensor(trigPin, echoPin);

  conectarWiFi(ssid, password, &conectado); 
  sincronizarHorarioNTP();      

  configurarMQTT(mqtt_server, mqtt_port, mqtt_user, mqtt_password);
  conectarMQTT();            

  tentarEnviarLogsPendentes();
  publicarDadosSensor(&conectado);
}

void loop() {
  static unsigned long ultimaLeitura = 0;
  unsigned long agora = millis();

  getMQTTClient().loop();

  if (agora - ultimaLeitura >= intervalo) {
    ultimaLeitura = agora;
    publicarDadosSensor(&conectado);
  }

  if (!conectado) {
    reconectarWiFi(ssid, password, &conectado);
    if (conectado) {
      sincronizarHorarioNTP();  
      configurarMQTT(mqtt_server, mqtt_port, mqtt_user, mqtt_password);
      conectarMQTT();  
    }
  }
}
