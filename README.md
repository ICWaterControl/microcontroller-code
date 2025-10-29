# 🌊 Projeto de Monitoramento de Lâmina d’Água com ESP32

Este repositório contém o firmware de um sistema embarcado para o monitoramento em tempo real do nível de uma lâmina d’água, desenvolvido como parte de um projeto de Iniciação Científica no SENAI CIMATEC.

---

## 🎯 Sobre o Projeto

O objetivo principal é desenvolver uma solução IoT de baixo custo para medir o nível da água utilizando um sensor ultrassônico e transmitir esses dados para a nuvem. O sistema se conecta a uma rede Wi-Fi, envia os dados formatados em JSON para um broker MQTT (HiveMQ Cloud) e inclui funcionalidades de resiliência, como armazenamento local de dados em caso de falha de conexão.

---

## ✨ Funcionalidades Principais

-   **Medição de Nível:** Leitura da distância da lâmina d’água com sensor ultrassônico.
-   **Conectividade:** Conexão a redes Wi-Fi com tratamento de reconexão automática.
-   **Comunicação MQTT:** Envio de dados para o broker HiveMQ Cloud usando protocolo seguro (TLS/SSL).
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
    git clone https://github.com/seu-usuario/seu-repositorio.git
    cd seu-repositorio
    ```

2.  **Abra o projeto no VS Code:**
    -   Com o VS Code aberto, clique em `File > Open Folder...` e selecione a pasta do projeto.
    -   O PlatformIO irá reconhecer o `platformio.ini` e configurar o ambiente.

3.  **Configure as credenciais:**
    -   Abra o arquivo `src/main.cpp`.
    -   Localize e altere as seguintes variáveis com as suas informações:

    ```cpp
    // --- Configurações da Rede e Servidores ---

    // Credenciais da Rede Wi-Fi
    const char* ssid = "SUA_REDE_WIFI";
    const char* password = "SUA_SENHA_WIFI";

    // Configurações do Broker MQTT (HiveMQ Cloud ou outro)
    const char* mqtt_server    = "SEU_BROKER_URL";
    const int   mqtt_port      = 8883;
    const char* mqtt_user      = "SEU_USUARIO_MQTT";
    const char* mqtt_password  = "SUA_SENHA_MQTT";

    // Pinos do Sensor Ultrassônico (ajuste se necessário)
    const int trigPin = 5;
    const int echoPin = 18;
    ```

4.  **Compile e envie para o ESP32:**
    -   Conecte o ESP32 ao seu computador.
    -   Na barra de status do PlatformIO (canto inferior do VS Code), clique no ícone de seta (→) para compilar e fazer o upload do firmware.

5.  **Monitore a execução:**
    -   Após o upload, clique no ícone de tomada (🔌) para abrir o Monitor Serial e acompanhar as mensagens de log do dispositivo.

---

## 📂 Estrutura do Repositório

```
.
├── include/                # Arquivos de cabeçalho (.h)
│   ├── mqtt_manager.h
│   ├── publish_manager.h
│   ├── ultrasonic_sensor.h
│   └── wifi_manager.h
├── lib/                    # Bibliotecas locais (se houver)
├── src/                    # Código-fonte (.cpp)
│   ├── main.cpp            # Ponto de entrada e lógica principal
│   ├── mqtt_manager.cpp
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

-   **Broker:** HiveMQ Cloud (configurável)
-   **Protocolo:** MQTT com TLS/SSL
-   **Formato da Mensagem:** JSON

**Exemplo de payload:**
```json
{
  "timestamp": "2025-10-29T10:30:00Z",
  "altura_lamina_agua": 12.5
}
```

---

## 👥 Equipe

| Função | Nome | LinkedIn |
| :--- | :--- | :--- |
| **Orientador** | Dr. Márcio Sousa | - |
| **Desenvolvedor** | Pedro Henrique Mascarenhas de Andrade | [LinkedIn](https://www.linkedin.com/in/pedroandrade07/) |
| **Desenvolvedor** | Gustavo Maia | [LinkedIn](https://www.linkedin.com/in/gustavomaia-dev/) |

---

## ⚖️ Licença

Este projeto está licenciado sob a Licença MIT.
