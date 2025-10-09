#include "../include/ultrasonic_sensor.h"
#include "../include/publish_manager.h"
#include "../include/mqtt_manager.h"
#include "../include/wifi_manager.h"

// --- Configuração e Constantes ---
static int trigPin, echoPin;
static const char* topico_distancia = "sensor/distancia";

// Constantes para legibilidade e fácil ajuste
const int TRIGGER_PULSE_DURATION_US = 15;
const int PULSEIN_TIMEOUT_US = 26000;
const float SOUND_SPEED_DIVISOR = 58.0;

// --- Funções do Sensor ---
void configurarSensor(int trig, int echo) {
    trigPin = trig;
    echoPin = echo;
    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT_PULLUP);
}

long lerDistancia() {
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(TRIGGER_PULSE_DURATION_US);
    digitalWrite(trigPin, LOW);

    long duration = pulseIn(echoPin, HIGH, PULSEIN_TIMEOUT_US);
    return (duration == 0) ? -1 : duration / SOUND_SPEED_DIVISOR;
}

void publicarDadosSensor(bool* conectado) {
    long distancia = lerDistancia();

    if (distancia < 0) {
        publishMessage("Erro na leitura do sensor", "ERROR", topico_distancia);
    } else {
        
        String payload = String(distancia);

        if (getMQTTClient().connected()) {
            publishMessage("Distancia lida publicada via MQTT: " + payload, "SUCCESS", topico_distancia, distancia);
        } else {
            *conectado = false;
            publishMessage("MQTT offline. Salvando distancia: " + payload, "ERROR", topico_distancia, distancia);
        }
    }
    
    tentarEnviarLogsPendentes();
}