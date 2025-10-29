/**
 * @file main.cpp
 * @brief Ponto de entrada principal e orquestrador do sistema embarcado.
 * @details Este arquivo contém as funções `setup()` e `loop()`, que são o coração de qualquer programa
 *          baseado em Arduino/ESP-IDF. A função `setup` é responsável por inicializar todos os módulos
 *          e hardware, enquanto a `loop` executa a lógica principal do sistema de forma contínua, garantindo
 *          a leitura dos sensores, a manutenção da conectividade e a publicação dos dados.
 */

 #include "../include/main.h"

// --- Configurações da Rede e Servidores ---

// Credenciais da Rede Wi-Fi
const char* ssid = "Wokwi-GUEST";
const char* password = "";

// Configurações do Broker MQTT
const char* mqtt_server    = "0bbdda7fb11e4c4795c3e07e3ac1ff60.s1.eu.hivemq.cloud";
const int   mqtt_port      = 8883;
const char* mqtt_user      = "Pedro";
const char* mqtt_password  = "Luciene.456";

// --- Pinos e Constantes Globais ---

// Pinos do Sensor Ultrassônico
const int trigPin = 5; // Pino de disparo (trigger)
const int echoPin = 18; // Pino de eco (echo)

// Variável global para monitorar o estado da conexão Wi-Fi
static bool conectado;

// Intervalo entre as leituras e publicações dos dados do sensor (em milissegundos)
const unsigned long intervalo = 5000; // Ex: 5 segundos

/**
 * @brief Função de inicialização do sistema.
 * @details Esta função é executada uma única vez quando o ESP32 é ligado ou resetado. Sua principal
 *          responsabilidade é preparar o ambiente de execução. Ela inicializa a comunicação serial para
 *          depuração, o sistema de arquivos SPIFFS para logging offline, configura os pinos do sensor,
 *          estabelece a conexão Wi-Fi, sincroniza o tempo com um servidor NTP, configura a conexão MQTT
 *          e, finalmente, realiza uma primeira tentativa de enviar logs antigos e publica a primeira
 *          leitura do sensor.
 */
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

/**
 * @brief Função de loop principal do sistema.
 * @details Esta função é o coração do programa, executada repetidamente após a conclusão da `setup()`.
 *          Suas responsabilidades são:
 *          1. Manter o cliente MQTT ativo, processando mensagens de entrada e mantendo a conexão viva (`getMQTTClient().loop()`).
 *          2. Verificar periodicamente se o intervalo de leitura do sensor foi atingido para realizar e publicar uma nova medição.
 *          3. Monitorar o estado da conexão Wi-Fi (`conectado`). Se a conexão for perdida, ela tentará se reconectar
 *             e, uma vez reconectada, reiniciará a sincronização de tempo e a conexão MQTT.
 *          Esta abordagem garante que o sistema seja resiliente a falhas de rede.
 */
void loop() {
  static unsigned long ultimaLeitura = 0;
  unsigned long agora = millis();

  getMQTTClient().loop();

  if (agora - ultimaLeitura >= intervalo) {
    ultimaLeitura = agora;
    publicarDadosSensor(&conectado);
  }

  /* Modularizar esse trecho */
  if (!conectado) {
    reconectarWiFi(ssid, password, &conectado);
    if (conectado) {

      /* Modularizar esse trecho */
      char message[128];
      if (sizeof(ssid) > 108) {
        ssid = "SSID muito longo";
      }
      snprintf(message, sizeof(message), "Reconectado na rede: %s", ssid);

      publishMessage(String(message), "SUCCESS", "sistema/comunicacao/wifi");
      sincronizarHorarioNTP();  
      configurarMQTT(mqtt_server, mqtt_port, mqtt_user, mqtt_password);
      conectarMQTT();  
    }
  }
}