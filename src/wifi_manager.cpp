/**
 * @file wifi_manager.cpp
 * @brief Implementação da classe WifiManager.
 * @details Contém a lógica de conexão inicial via portal captivo (WiFiManager library),
 *          reconexão automática e sincronização do relógio com NTP. Todos os eventos
 *          são registrados usando o PublishManager quando disponível.
 */

#include <WiFiManager.h>
#include <esp_wifi.h>
#include <ESP32Ping.h>

#include "../include/wifi_manager.h"
#include "../include/publish_manager.h"

void WifiManager::setPublishManager(PublishManager *pm)
{
    m_publishManager = pm;
}

/**
 * @brief Implementação da conexão inicial com a rede Wi-Fi.
 * @details Usa o WiFiManager (portal captivo) para autoconnect. Se a conexão falhar,
 *          registra um log de erro. Se bem-sucedida, registra o SSID e ativa o modo
 *          de economia de energia mínimo do modem WiFi.
 */
void WifiManager::begin()
{
    ::WiFiManager wm;
    // wm.resetSettings(); // Descomente para limpar as configurações salvas

    // Configura o IP estático no WiFiManager
    /*IPAddress _ip      = IPAddress(192, 168, 1, 184);
    IPAddress _gw      = IPAddress(192, 168, 1, 1);
    IPAddress _sn      = IPAddress(255, 255, 255, 0);
    wm.setSTAStaticIPConfig(_ip, _gw, _sn);*/

    bool res = wm.autoConnect("CaixaDagua_AP");
    if (!res)
    {
        if (m_publishManager)
            m_publishManager->publicarLogSistema("Falha ao conectar ou tempo de configuração esgotado", "ERROR");
        m_conectado = false;
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
        if (m_publishManager)
            m_publishManager->publicarLogSistema(String(message), "SUCCESS");
        m_conectado = true;

        esp_wifi_set_ps(WIFI_PS_MIN_MODEM);
    }
}

/**
 * @brief Verifica o estado atual da conexão e tenta reconectar quando necessário.
 */
void WifiManager::reconectar(unsigned long timeoutMs)
{
    if (WiFi.status() == WL_CONNECTED)
    {
        m_conectado = true;
        return;
    }

    if (timeoutMs == 0)
    {
        m_conectado = false;
        if (m_publishManager)
            m_publishManager->publicarLogSistema("Orcamento de rede esgotado antes da reconexao WiFi", "ERROR");
        return;
    }

    m_conectado = false;
    WiFi.reconnect();

    const unsigned long start = millis();

    while (WiFi.status() != WL_CONNECTED && (millis() - start) < timeoutMs)
    {
        delay(250);
    }

    if (WiFi.status() == WL_CONNECTED)
    {
        m_conectado = true;
        if (m_publishManager)
            m_publishManager->publicarLogSistema("WiFi reconectado com sucesso", "SUCCESS");
    }
    else
    {
        if (m_publishManager)
            m_publishManager->publicarLogSistema("Falha ao reconectar WiFi", "ERROR");
    }
}

/**
 * @brief Implementação da sincronização de tempo com NTP.
 * @details Configura o cliente NTP com fuso horário UTC-3 e tenta obter a hora
 *          dentro do timeout especificado.
 */
bool WifiManager::sincronizarNTP(unsigned long timeoutMs)
{
    if (timeoutMs == 0)
    {
        if (m_publishManager)
            m_publishManager->publicarLogSistema("Orcamento de rede esgotado antes da sincronizacao NTP", "ERROR");
        return false;
    }

    configTime(-3 * 3600, 0, "pool.ntp.org", "time.nist.gov");
    struct tm timeinfo;
    const unsigned long start = millis();

    while ((millis() - start) < timeoutMs)
    {
        if (getLocalTime(&timeinfo))
        {
            Serial.print("Horário atualizado: ");
            Serial.println(&timeinfo, "%Y-%m-%d %H:%M:%S");
            /*if (m_publishManager)
                m_publishManager->publicarLogSistema("Tempo NTP sincronizado com sucesso", "SUCCESS");*/
            return true;
        }
        delay(1000);
    }
    /*if (m_publishManager)
        m_publishManager->publicarLogSistema("Falha ao obter tempo via NTP", "ERROR");*/
    return false;
}

bool WifiManager::isConectado() const
{
    return m_conectado;
}

bool WifiManager::ping(const char *addr) const
{
    return Ping.ping(addr, 3);
}