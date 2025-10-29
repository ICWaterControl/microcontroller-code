/**
 * @file wifi_manager.h
 * @brief Cabeçalho do gerenciador de conexão Wi-Fi e sincronização de tempo (NTP).
 * @details Este arquivo define a interface para todas as funcionalidades relacionadas à rede Wi-Fi no ESP32.
 *          Ele abstrai a complexidade da biblioteca WiFi.h do Arduino, fornecendo funções de alto nível para
 *          conectar, reconectar e verificar o status da conexão. Além disso, inclui a funcionalidade para
 *          sincronizar o relógio interno do dispositivo com um servidor de tempo da internet (NTP), o que é
 *          essencial para a precisão dos timestamps em logs e outras operações sensíveis ao tempo.
 */
#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include <WiFi.h>
#include <time.h>
#include <string.h>
#include <cstdio>

/**
 * @brief Estabelece a conexão inicial com a rede Wi-Fi.
 * @details Esta função deve ser chamada durante a inicialização do sistema. Ela inicia o processo de conexão
 *          com o ponto de acesso Wi-Fi usando o SSID e a senha fornecidos. A função opera de forma síncrona,
 *          ou seja, ela bloqueará a execução do programa até que a conexão seja estabelecida com sucesso.
 *          O progresso da conexão é impresso no monitor serial para fins de depuração. Ao final, o estado da
 *          conexão é refletido no ponteiro `conectado`.
 * @param ssid O nome (SSID) da rede Wi-Fi à qual se deseja conectar.
 * @param password A senha da rede Wi-Fi.
 * @param conectado Um ponteiro para uma variável booleana que será atualizada para `true` quando a conexão
 *                  for bem-sucedida. Isso permite que o estado da conexão seja compartilhado com outras
 *                  partes do sistema.
 */
void conectarWiFi(const char* ssid, const char* password, bool* conectado);

/**
 * @brief Verifica e, se necessário, restabelece a conexão com a rede Wi-Fi.
 * @details Esta função foi projetada para ser chamada periodicamente no loop principal do programa. Ela primeiro
 *          verifica o status atual da conexão Wi-Fi. Se a conexão foi perdida (`WL_CONNECTED` não é verdadeiro),
 *          ela tentará se reconectar automaticamente usando as mesmas credenciais (SSID e senha) fornecidas.
 *          Isso garante que o dispositivo possa se recuperar de perdas temporárias de sinal ou reinicializações
 *          do roteador sem a necessidade de uma reinicialização manual.
 * @param ssid O nome (SSID) da rede Wi-Fi.
 * @param password A senha da rede Wi-Fi.
 * @param conectado Um ponteiro para a variável booleana que armazena o estado da conexão. A função atualizará
 *                  este valor para `true` na reconexão ou o manterá como `false` se a tentativa falhar.
 */
void reconectarWiFi(const char* ssid, const char* password, bool* conectado);

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