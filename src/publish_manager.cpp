/**
 * @file publish_manager.cpp
 * @brief Implementação do gerenciador de publicação e logging.
 * @details Este arquivo contém a lógica de implementação para o sistema de logging e publicação. Ele lida com a
 *          criação de mensagens de log em formato JSON, a inicialização do sistema de arquivos SPIFFS, a publicação
 *          de mensagens via MQTT e o armazenamento local de logs quando a rede está indisponível.
 */

#include "../include/publish_manager.h"
#include "../include/mqtt_manager.h"

// Tópico MQTT para onde os logs pendentes serão enviados.
static const char* topico_pendentes = "sistema/pendentes";

/**
 * @brief Cria uma string de log formatada em JSON.
 * @details Esta função interna é usada para construir a string JSON para cada log. Ela coleta o timestamp atual,
 *          a mensagem, o status e um valor de distância opcional. A estrutura do JSON é padronizada para facilitar
 *          o processamento por sistemas de backend. Se a obtenção do tempo local falhar, um valor de erro
 *          é inserido no campo de timestamp.
 * @param mensagem A mensagem principal do log.
 * @param status O status ou nível do log (ex: "INFO", "ERROR").
 * @param distancia Um valor de distância opcional a ser incluído. Se negativo, não é adicionado ao JSON.
 * @return Uma `String` contendo o objeto JSON completo do log.
 */
String criarJsonLog(const String& mensagem, const String& status, int distancia = -1) {
  JsonDocument doc;
  struct tm timeinfo;

  if (getLocalTime(&timeinfo)) {
    char timeString[64];
    strftime(timeString, sizeof(timeString), "%Y-%m-%d %H:%M:%S", &timeinfo);
    doc["timestamp"] = String(timeString);
  } else {
    doc["timestamp"] = "Erro ao obter horario";
  }

  // Adiciona os campos ao JSON
  doc["status"] = status;
  doc["origem"] = "esp32";
  doc["mensagem"] = mensagem;

  if (distancia >= 0) {
    doc["distancia"] = distancia;
  }

  String resultado;
  serializeJson(doc, resultado);
  return resultado;
}

/**
 * @brief Inicializa o sistema de arquivos SPIFFS.
 * @details Implementação da função que monta o sistema de arquivos flash SPIFFS. A flag `true` em `SPIFFS.begin(true)`
 *          formata o sistema de arquivos se a montagem falhar, o que pode ser útil na primeira execução ou se o
 *          sistema de arquivos for corrompido. Uma falha na montagem é um erro crítico para a persistência de logs.
 * @return `true` se a inicialização for bem-sucedida, `false` caso contrário.
 */
bool iniciarSPIFFS() {
  if (!SPIFFS.begin(true)) {
    Serial.println("[SPIFFS] Falha ao montar SPIFFS");
    return false;
  }
  return true;
}

/**
 * @brief Publica uma mensagem via MQTT ou a salva localmente.
 * @details Esta função primeiro cria a mensagem de log JSON chamando `criarJsonLog`. Em seguida, tenta publicar
 *          a mensagem no tópico MQTT especificado. Se o cliente MQTT estiver conectado e a publicação for bem-sucedida, 
 *          um log de sucesso é impresso. Caso contrário, a função abre (ou cria) o arquivo `/log.txt` no SPIFFS
 *          e anexa a mensagem de log a ele, garantindo que a informação não seja perdida.
 * @param mensagem A mensagem principal a ser publicada.
 * @param status O status do log.
 * @param topico O tópico MQTT para a publicação.
 * @param distancia O valor de distância opcional.
 */
void publishMessage(const String& mensagem, const String& status, const char* topico, int distancia) {
  String logStr = criarJsonLog(mensagem, status, distancia);

  Serial.println(logStr); // Imprime log no monitor serial (depuração)

  auto& client = getMQTTClient(); 

  // Tentar enviar o log via MQTT
  if (client.connected() && client.publish(topico, logStr.c_str())) {
    Serial.println("[MQTT] Log enviado com sucesso.");
  } else {
    // Se falhar, salva o log no memória Flash
    File file = SPIFFS.open("/log.txt", FILE_APPEND);
    if (file) {
      file.println(logStr);
      file.close();
      Serial.println("[SPIFFS] Log salvo localmente.");
    } else {
      Serial.println("[SPIFFS] Erro ao salvar log.");
    }
  }
}

/**
 * @brief Tenta reenviar logs pendentes do SPIFFS.
 * @details Esta função implementa a lógica de recuperação de logs. Ela lê o arquivo `/log.txt` linha por linha.
 *          Para cada linha (que é um log JSON), ela tenta publicar no tópico de logs pendentes. Se a publicação
 *          falhar, a linha é escrita em um arquivo temporário (`/temp_log.txt`). Após processar todas as linhas,
 *          o arquivo de log original é removido. Se alguma publicação falhou, o arquivo temporário é renomeado
 *          de volta para `/log.txt`, preservando os logs que não puderam ser enviados. Se tudo foi enviado com sucesso,
 *          o arquivo temporário é simplesmente removido.
 */
void tentarEnviarLogsPendentes() {
  auto& client = getMQTTClient(); 
  
  if (!SPIFFS.exists("/log.txt")) return;

  File file = SPIFFS.open("/log.txt", "r");
  File tempFile = SPIFFS.open("/temp_log.txt", FILE_WRITE);

  if (!file || !tempFile) {
    if (file) file.close();
    return;
  }

  bool algumFalhou = false;
  while (file.available()) {
    String linha = file.readStringUntil('\n');
    linha.trim();
    if (linha.length() < 10) continue;

    if (client.connected()) {
      if (!client.publish(topico_pendentes, linha.c_str())) {
        tempFile.println(linha);
        algumFalhou = true;
      }
    } else {
      tempFile.println(linha);
      algumFalhou = true;
    }
  }

  file.close();
  tempFile.close();
  SPIFFS.remove("/log.txt");

  if (algumFalhou) {
    SPIFFS.rename("/temp_log.txt", "/log.txt");
  } else {
    SPIFFS.remove("/temp_log.txt");
  }
}