#ifndef GOOGLE_SHEETS_PUBLISHER_H
#define GOOGLE_SHEETS_PUBLISHER_H

#include <Arduino.h>

/**
 * @brief Envia uma string JSON para o backend do Google Sheets.
 * @details Esta função realiza uma requisição HTTP POST para a URL pré-configurada
 *          do Google Apps Script, enviando a string JSON no corpo da requisição.
 *          A função lida com a conexão Wi-Fi e imprime o status da operação no
 *          Serial monitor.
 * 
 * @param jsonPayload A string JSON a ser enviada.
 */
void publishToGoogleSheets(const String& jsonPayload);

#endif
