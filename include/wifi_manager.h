#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include <WiFi.h>
#include <time.h>

/**
 * @brief Estabelece a conexão inicial com a rede Wi-Fi.
 * @details Esta função deve ser chamada durante a inicialização do sistema. Ela inicia o processo de conexão
 *          com o ponto de acesso Wi-Fi usando o SSID e a senha fornecidos. A função opera de forma síncrona,
 *          ou seja, ela bloqueará a execução do programa até que a conexão seja estabelecida com sucesso.
 *          O progresso da conexão é impresso no monitor serial para fins de depuração. Ao final, o estado da
 *          conexão é refletido no ponteiro `conectado`.
 * @param conectado Um ponteiro para uma variável booleana que será atualizada para `true` quando a conexão
 *                  for bem-sucedida. Isso permite que o estado da conexão seja compartilhado com outras
 *                  parte do sistema.
 */
void conectarWiFi(bool& conectado);

/**
 * @brief Verifica e, se necessário, restabelece a conexão com a rede Wi-Fi.
 * @details Esta função foi projetada para ser chamada periodicamente no loop principal do programa. Ela primeiro
 *          verifica o status atual da conexão Wi-Fi. Se a conexão foi perdida (`WL_CONNECTED` não é verdadeiro),
 *          ela tentará se reconectar automaticamente usando as mesmas credenciais (SSID e senha) fornecidas.
 *          Isso garante que o dispositivo possa se recuperar de perdas temporárias de sinal ou reinicializações
 *          do roteador sem a necessidade de uma reinicialização manual.
 * @param conectado Um ponteiro para a variável booleana que armazena o estado da conexão. A função atualizará
 *                  este valor para `true` na reconexão ou o manterá como `false` se a tentativa falhar.
 */
void reconectarWiFi(bool& conectado);

/**
 * @brief Sincroniza o relógio do dispositivo com um servidor de tempo da rede (NTP).
 * @details Após uma conexão Wi-Fi bem-sucedida, esta função pode ser chamada para ajustar o relógio de tempo real
 *          interno do ESP32. Ela configura o cliente NTP com servidores públicos e um fuso horário específico (neste
 *          caso, UTC-3). A função faz várias tentativas para obter a hora. A sincronização do tempo é crucial
 *          para garantir que todos os logs e eventos registrados tenham um timestamp preciso e consistente.
 * @return `true` se a sincronização de tempo for bem-sucedida, `false` caso contrário.
 */
bool sincronizarHorarioNTP();

#endif