/**
 * @file wifi_manager.cpp
 * @brief Implementação do gerenciador de conexão Wi-Fi e sincronização de tempo.
 * @details Este arquivo contém a lógica de implementação para as funções de gerenciamento da rede Wi-Fi.
 *          Ele lida com a conexão inicial, a lógica de reconexão e a sincronização do relógio do sistema
 *          com um servidor NTP. Todos os eventos importantes, como sucesso ou falha na conexão e sincronização,
 *          são registrados usando o `publish_manager`.
 */

#include <WiFiManager.h>
#include "../include/wifi_manager.h"
#include "../include/publish_manager.h"

/**
 * @brief Implementação da função de conexão inicial com a rede Wi-Fi.
 * @details A função chama `WiFi.begin()` para iniciar o processo de conexão. Em seguida, entra em um
 *          loop `while` que bloqueia a execução até que o status da conexão seja `WL_CONNECTED`.
 *          Durante a espera, pontos são impressos no monitor serial para fornecer feedback visual.
 *          Após a conexão bem-sucedida, uma mensagem de sucesso é publicada e a flag de estado
 *          `conectado` é atualizada para `true`.
 * @param conectado Ponteiro para a flag de estado da conexão.
 */
void conectarWiFi(bool& conectado)
{
  WiFiManager wm;
  //wm.resetSettings(); // Descomente para limpar as configurações salvas

  bool res = wm.autoConnect("CaixaDagua_AP");
  if (!res)
  {
    publicarLogSistema("Falha ao conectar ou tempo de configuração esgotado", "ERROR");
    conectado = false;
  }
  else
  {
    char message[128];
    const char *ssid = WiFi.SSID().c_str();
    if (strlen(ssid) > 108)
    {
      ssid = "SSID muito longo";
    }
    snprintf(message, sizeof(message), "Conectado na rede: %s", ssid);
    publicarLogSistema(String(message), "SUCCESS");
    conectado = true;
  }
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
bool sincronizarHorarioNTP()
{
  configTime(-3 * 3600, 0, "pool.ntp.org", "time.nist.gov");
  struct tm timeinfo;
  for (int i = 0; i < 10; i++)
  {
    if (getLocalTime(&timeinfo))
    {
      publicarLogSistema("Tempo NTP sincronizado com sucesso", "SUCCESS");
      return true;
    }
    delay(1000);
  }
  publicarLogSistema("Falha ao obter tempo via NTP", "ERROR");
  return false;
}