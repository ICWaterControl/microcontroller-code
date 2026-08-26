/**
 * @file main.cpp
 * @brief Ponto de entrada principal e orquestrador do sistema embarcado.
 * @details Este arquivo contém as funções `setup()` e `loop()`, que são o coração de qualquer programa
 *          baseado em Arduino/ESP-IDF. A função `setup` é responsável por inicializar todos os módulos
 *          e hardware, enquanto a `loop` executa a lógica principal do sistema de forma contínua, garantindo
 *          a leitura dos sensores, a manutenção da conectividade e a publicação dos dados.
 *
 *          Na versão orientada a objetos, cada módulo é encapsulado em uma classe com estado privado
 *          e interface pública. Os objetos são instanciados globalmente e conectados via referências
 *          e injeção de dependência. O fluxo de inicialização e deep sleep permanece idêntico
 *          à versão procedural.
 */

#include "../include/main.h"
#include "../include/aws_iot_config.h"

// --- Configurações da Rede e Servidores ---

// Configurações do Broker MQTT (AWS IoT Core)
const int mqtt_port = 8883;

// --- Pinos e Constantes Globais ---

// Pinos do Sensor Ultrassônico
const uint8_t trigPin = 5;  // Pino de disparo (trigger)
const uint8_t echoPin = 18; // Pino de eco (echo)

// Intervalo entre as leituras e publicações dos dados do sensor (em segundos)
#define uS_TO_S_FACTOR 1000000ULL
constexpr int SLEEP_TIME_IN_SECONDS = 1800; // 30 minutos
constexpr int TIME_TO_SLEEP = SLEEP_TIME_IN_SECONDS * uS_TO_S_FACTOR;
constexpr unsigned long NETWORK_BUDGET_MS = 12000;
constexpr uint32_t NTP_SYNC_EVERY_CYCLES = 12;

// Mantem o contador entre despertares de deep sleep.
RTC_DATA_ATTR uint32_t wakeCycleCounter = 0;

// --- Instâncias dos Objetos (Composição) ---

UltrasonicSensor ultrasonicSensor;
BatterySensor    batterySensor;
MqttManager      mqttManager;
MqttPublisher    mqttPublisher(mqttManager);
PublishManager   publishManager(mqttPublisher, ultrasonicSensor, batterySensor);
WifiManager      wifiManager;

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
void setup()
{
  setCpuFrequencyMhz(80);

  // Serial.begin(115200);
  delay(10);

  // Serial.println("ESP32 is running");

  // Injeta dependências circulares após a construção de todos os objetos
  mqttManager.setPublishManager(&publishManager);
  wifiManager.setPublishManager(&publishManager);

  // batterySensor.begin();
  publishManager.iniciarSPIFFS();
  // ultrasonicSensor.begin(trigPin, echoPin);

  wakeCycleCounter++;
  const bool shouldSyncNtp =
      (wakeCycleCounter == 1) || (wakeCycleCounter % NTP_SYNC_EVERY_CYCLES == 0);

  const unsigned long networkWindowStart = millis();
  auto remainingNetworkBudgetMs = [networkWindowStart]() -> unsigned long {
    const unsigned long elapsed = millis() - networkWindowStart;
    return (elapsed >= NETWORK_BUDGET_MS) ? 0 : (NETWORK_BUDGET_MS - elapsed);
  };

  wifiManager.begin(); // Conecta WiFi
  if (!wifiManager.isConectado())
  {
    wifiManager.reconectar(remainingNetworkBudgetMs());
  }
  btStop(); // Desativa o Bluetooth

  if (wifiManager.isConectado() && shouldSyncNtp)
  {
    const unsigned long timeoutNtp = remainingNetworkBudgetMs();
    if (timeoutNtp > 0)
    {
      wifiManager.sincronizarNTP(timeoutNtp);
    }
    else
    {
      publishManager.publicarLogSistema("Orcamento de rede esgotado; NTP sera ignorado neste ciclo", "ERROR");
    }
  }
  else if (!wifiManager.isConectado())
  {
    publishManager.publicarLogSistema("WiFi indisponivel; NTP sera ignorado neste ciclo", "ERROR");
  }

  mqttManager.configurar(AWS_IOT_ENDPOINT, mqtt_port);
  bool mqttConectado = false;
  if (wifiManager.isConectado())
  {
    const unsigned long timeoutMqtt = remainingNetworkBudgetMs();
    if (timeoutMqtt > 0)
    {
      mqttConectado = mqttManager.conectar(timeoutMqtt);
    }
    else
    {
      publishManager.publicarLogSistema("Orcamento de rede esgotado; MQTT sera ignorado neste ciclo", "ERROR");
    }
  }

  if (mqttConectado && remainingNetworkBudgetMs() > 0)
  {
    mqttPublisher.enviarLogsPendentes();
  }
  publishManager.publicarDistancia();
  publishManager.publicarBateria();

  batterySensor.sleep();

  // Serial.println("Entrando em modo deep sleep por 30 minutos...");
  gpio_deep_sleep_hold_en();
  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP);
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
void loop()
{
}