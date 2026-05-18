#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "secrets.h"

// Monta o link da API usando a chave
String openWeatherUrl = "http://api.openweathermap.org/data/2.5/weather?q=Fortaleza,BR&units=metric&appid=" + String(OPENWEATHER_API_KEY);

void obterClima() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    
    Serial.println("Consultando o clima no OpenWeatherMap...");
    http.begin(openWeatherUrl); 
    
    int codigoResposta = http.GET(); 

    if (codigoResposta > 0) {
      String payload = http.getString();
      JsonDocument doc; 
      DeserializationError erro = deserializeJson(doc, payload);

      if (!erro) {
        float temperatura = doc["main"]["temp"];
        int umidade = doc["main"]["humidity"];
        
        Serial.println("--- DADOS DO CLIMA ---");
        Serial.print("Temperatura: ");
        Serial.print(temperatura);
        Serial.println(" C");
        
        Serial.print("Umidade: ");
        Serial.print(umidade);
        Serial.println(" %");
        Serial.println("----------------------");
      } else {
        Serial.println("Erro ao traduzir os dados da API.");
      }
    } else {
      Serial.print("Erro na conexão HTTP: ");
      Serial.println(codigoResposta);
    }
    http.end(); 
  } else {
    Serial.println("Sem Wi-Fi! Impossível checar o clima.");
  }
}

void setup() {
  Serial.begin(115200);
  delay(1000); 

  Serial.print("\nConectando na rede: ");
  Serial.println(WIFI_SSID);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi Conectado!");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  // Chama a função do clima logo após conectar
  obterClima();
}

void loop() {
  // Deixaremos o loop vazio por enquanto. 
  // Mais tarde colocaremos um timer aqui para atualizar o clima a cada 10 minutos.
}