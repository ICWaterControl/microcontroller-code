/**
 * @file mqtt_manager.cpp
 * @brief Implementação do gerenciador de conexão MQTT.
 * @details Este arquivo contém a lógica de implementação para as funções declaradas em `mqtt_manager.h`.
 *          Ele gerencia um cliente MQTT global, lida com a configuração, o acesso à instância do cliente
 *          e a lógica de conexão/reconexão com o broker MQTT. A comunicação é feita de forma segura
 *          usando `WiFiClientSecure`, e os eventos de conexão são registrados através do `publish_manager`.
 */

#include "../include/mqtt_manager.h"
#include "../include/publish_manager.h"

#include <WiFiClientSecure.h>

// --- Variáveis Estáticas Globais ---

// Instância do cliente Wi-Fi seguro, usado como camada de transporte para o MQTT.
static WiFiClientSecure espClient;
// Instância global do cliente MQTT (PubSubClient), que opera sobre o cliente Wi-Fi.
static PubSubClient client(espClient);

/**
 * @brief Variáveis estáticas para armazenar as credenciais e configurações do broker.
 * @details
 *   - `static`: Limita a visibilidade destas variáveis apenas a este arquivo (`mqtt_manager.cpp`),
 *     funcionando como variáveis "privadas" do módulo.
 *   - `const char*`: Declara um ponteiro (`*`) para um caractere (`char`) constante (`const`).
 *     Em C/C++, `char*` é a forma clássica de se trabalhar com strings (textos), onde o ponteiro
 *     aponta para o primeiro caractere da sequência. O `const` indica que o conteúdo da string
 *     (o texto em si) não deve ser modificado.
 *
 *   - `_user`: Ponteiro que armazenará o endereço da string do nome de usuário.
 *   - `_password`: Ponteiro que armazenará o endereço da string da senha.
 *   - `_server`: Ponteiro que armazenará o endereço da string do servidor.
 *   - `_port`: Variável inteira para a porta do servidor.
 */
static const char* _user;
static const char* _password;
static const char* _server;
static int _port;

// Tópico para logs relacionados ao status da conexão MQTT.
static const char* topico = "sistema/comunicacao/mqtt";

/**
 * @brief Implementação da função de configuração do cliente MQTT.
 * @details Esta função armazena as informações do broker em variáveis estáticas locais e configura
 *          a instância do `PubSubClient`. A chamada `espClient.setInsecure()` é usada para pular a
 *          validação do certificado do servidor, facilitando a configuração em ambientes de teste.
 *          Em um ambiente de produção, seria necessário carregar um certificado CA.
 *          `client.setServer()` informa à biblioteca o endereço e a porta do broker a ser usado.
 * @param server Endereço do broker MQTT.
 * @param port Porta do broker.
 * @param user Nome de usuário para autenticação.
 * @param password Senha para autenticação.
 */
void configurarMQTT(const char* server, int port, const char* user, const char* password) {
  _server = server;
  _port = port;
  _user = user;
  _password = password;

  espClient.setInsecure();
  client.setServer(_server, _port);
}

/**
 * @brief Implementação da função que retorna a instância do cliente MQTT.
 * @details Simplesmente retorna uma referência para a instância estática global `client`. O uso de
 *          uma função getter como esta é uma boa prática de encapsulamento, permitindo que o resto
 *          do código acesse o cliente sem ter conhecimento direto da variável global estática.
 * @return Referência para a instância do `PubSubClient`.
 */
PubSubClient& getMQTTClient() {
  return client;
}

/**
 * @brief Implementação da lógica de conexão com o broker MQTT.
 * @details Esta função entra em um loop que persiste até que a conexão seja estabelecida ou o timeout
 *          seja atingido. Dentro do loop, `client.connect()` é chamado com um ID de cliente e as
 *          credenciais. Se a conexão for bem-sucedida, uma mensagem de sucesso é publicada e a função
 *          retorna `true`. Se falhar, o estado do erro é registrado, o sistema aguarda 5 segundos e
 *          tenta novamente. Se o tempo total de tentativas exceder `timeoutMs`, uma mensagem de timeout
 *          é registrada e a função retorna `false`.
 * @param timeoutMs O tempo máximo de espera para a conexão em milissegundos.
 * @return `true` em caso de sucesso, `false` em caso de falha ou timeout.
 */
bool conectarMQTT(unsigned long timeoutMs) { // Timeout padrão 30s
  unsigned long start = millis();

  while (!client.connected()) {
    if (client.connect("ESP32Client", _user, _password)) {
      publishMessage("MQTT conectado com sucesso", "SUCCESS", topico);
      return true;
    } else {
      publishMessage("Falha ao conectar no MQTT. Código: " + String(client.state()), "ERROR", topico);
      delay(5000);
    }
    if (millis() - start > timeoutMs) {
      publishMessage("Timeout ao tentar conectar MQTT", "ERROR", topico);
      return false;
    }
  }
  return false;
}