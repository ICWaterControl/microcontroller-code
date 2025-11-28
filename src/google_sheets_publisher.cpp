#include "../include/google_sheets_publisher.h"
#include <HTTPClient.h>
#include <WiFi.h>

// URL do Google Apps Script para enviar os dados para a planilha.
// SUBSTITUA PELA URL GERADA NA IMPLANTAÇÃO DO SEU SCRIPT.
static const char* google_script_url = "https://script.google.com/macros/s/AKfycby3Ob2RLBDZfdu2G-IT_vzPilIxK0XCBeg1k_2ZrxrF1EkHI5PD8GMl5FPvqc4XgeRXKA/exec";

/**
 * @brief Envia a string JSON para o Google Sheets através de um Google Apps Script.
 * @param jsonPayload A string JSON a ser enviada.
 */
void publishToGoogleSheets(const String& jsonPayload) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    
    // ATENÇÃO: A URL do Google redireciona. O begin() precisa saber disso para seguir o redirecionamento.
    // Usar a versão do begin que aceita o certificado raiz do Google seria o ideal para produção.
    // Para simplificar, estamos confiando no redirecionamento HTTP.
    http.begin(google_script_url); 
    http.addHeader("Content-Type", "application/json");

    int httpResponseCode = http.POST(jsonPayload);

    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.println("[Google Sheets] HTTP Response code: " + String(httpResponseCode));
      Serial.println(response);
    } else {
      Serial.println("[Google Sheets] Error on sending POST: " + String(httpResponseCode));
    }

    http.end();
  } else {
    Serial.println("[Google Sheets] WiFi not connected, skipping send.");
  }
}
