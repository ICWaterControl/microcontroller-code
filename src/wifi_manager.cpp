/**
 * @file wifi_manager.cpp
 * @brief Implementação do gerenciador de conexão Wi-Fi e sincronização de tempo.
 * @details Este arquivo contém a lógica de implementação para as funções de gerenciamento da rede Wi-Fi.
 *          Ele lida com a conexão inicial, a lógica de reconexão e a sincronização do relógio do sistema
 *          com um servidor NTP. Todos os eventos importantes, como sucesso ou falha na conexão e sincronização,
 *          são registrados usando o `publish_manager`.
 */

#include <WiFi.h>
#include <time.h>

#include "../include/wifi_manager.h"
#include "../include/publish_manager.h"

// Tópico MQTT para logs relacionados ao status da conexão Wi-Fi e NTP.
static const char* topico = "sistema/comunicacao/wifi";

/**
 * @brief Implementação da função de conexão inicial com a rede Wi-Fi.
 * @details A função chama `WiFi.begin()` para iniciar o processo de conexão. Em seguida, entra em um
 *          loop `while` que bloqueia a execução até que o status da conexão seja `WL_CONNECTED`.
 *          Durante a espera, pontos são impressos no monitor serial para fornecer feedback visual.
 *          Após a conexão bem-sucedida, uma mensagem de sucesso é publicada e a flag de estado
 *          `conectado` é atualizada para `true`.
 * @param ssid O SSID da rede.
 * @param password A senha da rede.
 * @param conectado Ponteiro para a flag de estado da conexão.
 */
void conectarWiFi(const char* ssid, const char* password, bool* conectado) {
  WiFi.begin(ssid, password);
  Serial.print("[WiFi] Conectando");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  publishMessage("Wi-Fi conectado com sucesso", "SUCCESS", topico);
  *conectado = true;
}

/**
 * @brief Implementação da função de reconexão com a rede Wi-Fi.
 * @details Esta função é projetada para ser não-bloqueante. Ela simplesmente chama `WiFi.begin()`
 *          para iniciar uma nova tentativa de conexão em segundo plano e imediatamente atualiza a flag
 *          `conectado` para `true`. A verificação real do status da conexão e a espera são tratadas
 *          em outras partes do código (potencialmente no loop principal ou na próxima chamada que
 *          dependa da rede), permitindo que o resto do sistema continue funcionando sem travar.
 * @param ssid O SSID da rede.
 * @param password A senha da rede.
 * @param conectado Ponteiro para a flag de estado da conexão.
 */
void reconectarWiFi(const char* ssid, const char* password, bool* conectado) {
  WiFi.begin(ssid, password);
  *conectado = true;
}

/**
 * @brief Implementação da função de sincronização de tempo com NTP.
 * @details A função primeiro configura o cliente NTP usando `configTime`, definindo o deslocamento de fuso
 *          horário (UTC-3), o horário de verão (0) e os servidores NTP a serem usados. Em seguida, ela entra
 *          em um loop que tenta obter a hora local por até 10 vezes, com um intervalo de 1 segundo entre
 *          as tentativas. Se `getLocalTime()` for bem-sucedida, uma mensagem de sucesso é publicada e a
 *          função retorna `true`. Se todas as tentativas falharem, uma mensagem de erro é publicada e a
 *          função retorna `false`.
 * @return `true` se a sincronização for bem-sucedida, `false` caso contrário.
 */
bool sincronizarHorarioNTP() {
  configTime(-3 * 3600, 0, "pool.ntp.org", "time.nist.gov");
  struct tm timeinfo;
  for (int i = 0; i < 10; i++) {
    if (getLocalTime(&timeinfo)) {
      publishMessage("Tempo NTP sincronizado com sucesso", "SUCCESS",  topico);
      return true;
    }
    delay(1000);
  }
  publishMessage("Falha ao obter tempo via NTP", "ERROR",  topico);
  return false;
}