# 🌊 Projeto de Monitoramento de Lâmina d’Água com ESP32

Este repositório contém o firmware de um sistema embarcado para o monitoramento em tempo real do nível de uma lâmina d’água, desenvolvido como parte de um projeto de Iniciação Científica no SENAI CIMATEC.

---

## 🎯 Sobre o Projeto

O objetivo principal é desenvolver uma solução IoT de baixo custo para medir o nível da água utilizando um sensor ultrassônico e transmitir esses dados para a nuvem. O sistema se conecta a uma rede Wi-Fi, envia os dados formatados em JSON para o AWS IoT Core e inclui funcionalidades de resiliência, como armazenamento local de dados em caso de falha de conexão.

---

## ✨ Funcionalidades Principais

-   **Medição de Nível:** Leitura da distância da lâmina d’água com sensor ultrassônico.
-   **Conectividade:** Conexão a redes Wi-Fi com tratamento de reconexão automática.
-   **Comunicação MQTT:** Envio de dados para o AWS IoT Core usando protocolo seguro (TLS/SSL com certificado de dispositivo).
-   **Sincronização de Tempo:** Obtenção de timestamp via NTP para registros precisos.
-   **Formato de Dados:** Empacotamento das informações em formato JSON para fácil integração.
-   **Resiliência:** Armazenamento temporário de medições no sistema de arquivos SPIFFS em caso de falha de conexão, com envio posterior.

---

## 🛠️ Tecnologias Utilizadas

-   **Linguagem:** C/C++ (Arduino Framework)
-   **Microcontrolador:** ESP32
-   **Protocolo de Comunicação:** MQTT (com TLS)
-   **Sistema de Arquivos:** SPIFFS
-   **Sincronização de Tempo:** NTP

---

## 🔩 Hardware Necessário

-   Placa de desenvolvimento baseada no **ESP32**.
-   **Sensor Ultrassônico HC-SR04** (ou similar).
-   Cabos e protoboard para as conexões.

---

## 🚀 Começando

Siga os passos abaixo para compilar e executar o projeto.

### Pré-requisitos

-   **[Visual Studio Code](https://code.visualstudio.com/)**
-   **[Extensão PlatformIO IDE](https://platformio.org/platformio-ide)** instalada no VS Code.

### Instalação e Configuração

1.  **Clone o repositório:**
    ```bash
    git clone https://github.com/ICWaterControl/microcontroller-code.git
    cd microcontroller-code
    ```

2.  **Abra o projeto no VS Code:**
    -   Com o VS Code aberto, clique em `File > Open Folder...` e selecione a pasta do projeto.
    -   O PlatformIO irá reconhecer o `platformio.ini` e instalar as dependências automaticamente.

3.  **Configure as credenciais do AWS IoT Core:**
    -   Abra o arquivo `include/aws_iot_config.h`.
    -   Localize e altere os seguintes campos com os valores do seu ambiente:

    ```cpp
    static const char AWS_IOT_ENDPOINT[] = "SEU_ENDPOINT_DO_AWS_IOT_CORE";
    static const char AWS_IOT_CLIENT_ID[] = "SEU_CLIENT_ID";
    static const char AWS_IOT_ROOT_CA[] PROGMEM = R"EOF(... )EOF";
    static const char AWS_IOT_DEVICE_CERT[] PROGMEM = R"EOF(... )EOF";
    static const char AWS_IOT_PRIVATE_KEY[] PROGMEM = R"EOF(... )EOF";
    ```

    -   O firmware usa a porta `8883` por padrão para MQTT com TLS.

4.  **Compile e envie para o ESP32:**
    -   Conecte o ESP32 ao seu computador.
    -   Na barra de status do PlatformIO (canto inferior do VS Code), clique no ícone de seta (→) para compilar e fazer o upload do firmware.

5.  **Configure a rede Wi-Fi:**
    -   Após o primeiro boot, o ESP32 criará um ponto de acesso (AP) com o nome **`CaixaDagua_AP`**.
    -   Conecte-se a esta rede Wi-Fi com seu celular ou computador.
    -   Abra um navegador e acesse o endereço **`192.168.4.1`**.
    -   Uma página de configuração será exibida. Selecione a sua rede Wi-Fi, insira a senha e salve.
    -   O ESP32 irá se conectar à sua rede e reiniciar.

6.  **Monitore a execução:**
    -   Após o upload, clique no ícone de tomada (🔌) para abrir o Monitor Serial e acompanhar as mensagens de log do dispositivo.

---

## 📂 Estrutura do Repositório

```
.
├── include/                # Arquivos de cabeçalho (.h)
│   ├── mqtt_manager.h
│   ├── aws_iot_config.h
│   ├── battery_sensor.h
│   ├── mqtt_publisher.h
│   ├── publish_manager.h
│   ├── ultrasonic_sensor.h
│   └── wifi_manager.h
├── lib/                    # Bibliotecas locais (se houver)
├── src/                    # Código-fonte (.cpp)
│   ├── main.cpp            # Ponto de entrada e lógica principal
│   ├── battery_sensor.cpp
│   ├── mqtt_manager.cpp
│   ├── mqtt_publisher.cpp
│   ├── publish_manager.cpp
│   ├── ultrasonic_sensor.cpp
│   └── wifi_manager.cpp
├── test/                   # Testes (se houver)
├── .gitignore
├── diagram.json
├── platformio.ini          # Arquivo de configuração do PlatformIO
├── README.md
└── wokwi.toml
```

---

## 📡 Comunicação MQTT

-   **Broker:** AWS IoT Core (configurável)
-   **Protocolo:** MQTT com TLS/SSL
-   **Formato da Mensagem:** JSON

**Exemplo de payload:**
```json
{
    "timestamp": "2026-04-24 10:30:00",
    "status": "SUCCESS",
    "origem": "esp32",
    "mensagem": "Distancia lida: 123",
    "distancia": 123
}
```

---

## 📡 Tópicos MQTT

O sistema utiliza os seguintes tópicos para comunicação:

-   `sdk/test/java`: Publicação das medições de distância.
-   `sdk/test/python`: Publicação das medições de bateria.
-   `sdk/test/js`: Publicação de logs de sistema.

Mensagens não publicadas são armazenadas localmente em SPIFFS (`/log.txt`) e reenviadas no próximo ciclo com conectividade.

---

## 👥 Equipe

| Função | Nome | LinkedIn |
| :--- | :--- | :--- |
| **Orientador** | Dr. Márcio Sousa | [LinkedIn](https://www.linkedin.com/in/marcio-soussa/) |
| **Desenvolvedor** | Pedro Henrique Mascarenhas de Andrade | [LinkedIn](https://www.linkedin.com/in/pedrohmandrade/) |
| **Desenvolvedor** | Gustavo Maia | [LinkedIn](https://www.linkedin.com/in/gustavomaiajesus/) |

