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
#include "../include/aws_iot_config.h"

// --- Configurações da Rede e Servidores ---

// Credenciais da Rede Wi-Fi

// Configurações do Broker MQTT (AWS IoT Core)
const int mqtt_port = 8883;

// --- Pinos e Constantes Globais ---

// Pinos do Sensor Ultrassônico
const uint8_t trigPin = 5;  // Pino de disparo (trigger)
const uint8_t echoPin = 18; // Pino de eco (echo)

// Variável global para monitorar o estado da conexão Wi-Fi
static bool conectado;

// Intervalo entre as leituras e publicações dos dados do sensor (em segundos)
#define uS_TO_S_FACTOR 1000000ULL
constexpr int SLEEP_TIME_IN_SECONDS = 1800; // 30 minutos
constexpr int TIME_TO_SLEEP = SLEEP_TIME_IN_SECONDS * uS_TO_S_FACTOR;
constexpr unsigned long NETWORK_BUDGET_MS = 12000;
constexpr uint32_t NTP_SYNC_EVERY_CYCLES = 12;

// Mantem o contador entre despertares de deep sleep.
RTC_DATA_ATTR uint32_t wakeCycleCounter = 0;

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

  setupBatterySensor();
  iniciarSPIFFS();
  configurarSensor(trigPin, echoPin);

  wakeCycleCounter++;
  const bool shouldSyncNtp =
      (wakeCycleCounter == 1) || (wakeCycleCounter % NTP_SYNC_EVERY_CYCLES == 0);

  const unsigned long networkWindowStart = millis();
  auto remainingNetworkBudgetMs = [networkWindowStart]() -> unsigned long {
    const unsigned long elapsed = millis() - networkWindowStart;
    return (elapsed >= NETWORK_BUDGET_MS) ? 0 : (NETWORK_BUDGET_MS - elapsed);
  };

  conectarWiFi(conectado); // Conecta WiFi
  if (!conectado)
  {
    reconectarWiFi(conectado, remainingNetworkBudgetMs());
  }
  btStop(); // Desativa o Bluetooth

  if (conectado && shouldSyncNtp)
  {
    const unsigned long timeoutNtp = remainingNetworkBudgetMs();
    if (timeoutNtp > 0)
    {
      sincronizarHorarioNTP(timeoutNtp);
    }
    else
    {
      publicarLogSistema("Orcamento de rede esgotado; NTP sera ignorado neste ciclo", "ERROR");
    }
  }
  else if (!conectado)
  {
    publicarLogSistema("WiFi indisponivel; NTP sera ignorado neste ciclo", "ERROR");
  }

  configurarMQTT(AWS_IOT_ENDPOINT, mqtt_port);
  bool mqttConectado = false;
  if (conectado)
  {
    const unsigned long timeoutMqtt = remainingNetworkBudgetMs();
    if (timeoutMqtt > 0)
    {
      mqttConectado = conectarMQTT(timeoutMqtt);
    }
    else
    {
      publicarLogSistema("Orcamento de rede esgotado; MQTT sera ignorado neste ciclo", "ERROR");
    }
  }

  if (mqttConectado && remainingNetworkBudgetMs() > 0)
  {
    tentarEnviarLogsPendentes();
  }
  publicarLeituraDistancia(conectado);
  publicarLeituraBateria(conectado);

  sleepBaterrySensor();

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