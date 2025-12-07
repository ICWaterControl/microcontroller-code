/**
 * @file main.cpp
 * @brief Ponto de entrada principal e orquestrador do sistema embarcado.
 * @details Este arquivo contém as funções `setup()` e `loop()`, que são o coração de qualquer programa
 *          baseado em Arduino/ESP-IDF. A função `setup` é responsável por inicializar todos os módulos
 *          e hardware, enquanto a `loop` executa a lógica principal do sistema de forma contínua, garantindo
 *          a leitura dos sensores, a manutenção da conectividade e a publicação dos dados.
 */

 #include "../include/main.h"
 #include "../include/battery_sensor.h"
 #include "../include/publish_manager.h"
 #include "../include/mqtt_publisher.h"


// --- Configurações da Rede e Servidores ---

// Credenciais da Rede Wi-Fi

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
const int sleepTimeInSeconds = 1800; // Ex: 30 minutos

/**
 * @brief Função de inicialização do sistema.
 * @details Esta função é executada uma única vez quando o ESP32 é ligado ou resetado. Sua principal
 *          responsabilidade é preparar o ambiente de execução. Ela inicializa a comunicação serial para
 *          depuração, o sistema de arquivos SPIFFS para logging offline, configura os pinos do sensor,
 *          estabelece a conexão Wi-Fi, sincroniza o tempo com um servidor NTP, configura a conexão MQTT
 *          e, finalmente, realiza uma primeira tentativa de enviar logs antigos e publica a primeira
 *          leitura do sensor.
 *          No contexto do deep sleep, esta função é executada a cada despertar do dispositivo,
 *          garantindo que as conexões Wi-Fi e MQTT sejam restabelecidas antes de qualquer operação de publicação.
 */
void setup() {
  Serial.begin(115200);
  delay(500);

  Serial.println("ESP32 is running");

  setupBatterySensor();
  iniciarSPIFFS();
  configurarSensor(trigPin, echoPin);

  conectarWiFi(&conectado); 
  sincronizarHorarioNTP();      

  configurarMQTT(mqtt_server, mqtt_port, mqtt_user, mqtt_password);
  conectarMQTT();            

  tentarEnviarLogsPendentes();
  publicarLeituraDistancia(&conectado);
  publicarLeituraBateria(&conectado);
  Serial.println("Entrando em modo deep sleep por 30 minutos...");
  esp_sleep_enable_timer_wakeup(sleepTimeInSeconds * 1000000);
  esp_deep_sleep_start();
}

/**
 * @brief Função de loop principal do sistema.
 * @details Com a implementação do modo deep sleep, esta função não é utilizada,
 *          pois o dispositivo não fica em um loop contínuo. A lógica principal é executada
 *          na função `setup()` a cada despertar do dispositivo. As tentativas de reconexão
 *          Wi-Fi e MQTT, assim como outras rotinas de manutenção, são realizadas no `setup()`
 *          a cada ciclo de despertar.
 */
void loop() {
  // O loop fica vazio, pois o dispositivo estará em deep sleep.
}