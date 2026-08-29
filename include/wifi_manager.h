#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include <WiFi.h>
#include <time.h>

class PublishManager;

// Gerenciador de conexão Wi-Fi e sincronização NTP
class WifiManager
{
public:
    WifiManager() = default;

    // Injeta o PublishManager para logging (resolve dependência circular)
    void setPublishManager(PublishManager *pm);

    // Conecta via portal captivo (WiFiManager library)
    void begin();

    // Tenta reconectar dentro do timeout (ms)
    void reconectar(unsigned long timeoutMs = 15000);

    // Sincroniza relógio com NTP (UTC-3)
    bool sincronizarNTP(unsigned long timeoutMs = 10000);

    // Retorna true se conectado ao Wi-Fi
    bool isConectado() const;

    // Pinga no AWS IOT
    bool ping(const char *addr) const;

private:
    bool m_conectado = false;
    PublishManager *m_publishManager = nullptr;
};

#endif