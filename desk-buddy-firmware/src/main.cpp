#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <time.h>
#include "secrets.h"
#include "Tela.h"

#define PINO_BOTAO 15
#define PINO_BUZZER 21

String openWeatherUrl = "http://api.openweathermap.org/data/2.5/weather?q=Fortaleza,BR&units=metric&appid=" + String(OPENWEATHER_API_KEY);

unsigned long tempoUltimaConsulta = 0; 
const long intervaloConsulta = 600000;

float ultimaTemp = 0.0;
int ultimaUmid = 0;
bool climaCarregado = false;

bool modoRelogio = false; 
bool estadoAnteriorBotao = HIGH;

void bipar() {
  for (int i = 0; i < 150; i++) {
    digitalWrite(PINO_BUZZER, HIGH);
    delayMicroseconds(500);
    digitalWrite(PINO_BUZZER, LOW);
    delayMicroseconds(500);
  }
}

void obterClima() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(openWeatherUrl); 
    int codigoResposta = http.GET(); 

    if (codigoResposta > 0) {
      String payload = http.getString();
      JsonDocument doc; 
      DeserializationError erro = deserializeJson(doc, payload);

      if (!erro) {
        ultimaTemp = doc["main"]["temp"];
        ultimaUmid = doc["main"]["humidity"];
        climaCarregado = true;
        
        if (!modoRelogio) {
          mostrarTemperatura(ultimaTemp, ultimaUmid);
        }
      }
    }
    http.end(); 
  }
}

void setup() {
  Serial.begin(115200);

  pinMode(PINO_BOTAO, INPUT_PULLUP);
  pinMode(PINO_BUZZER, OUTPUT);
  digitalWrite(PINO_BUZZER, LOW);

  pinMode(32, OUTPUT);       
  digitalWrite(32, HIGH);    

  inicializarTela();

  Serial.print("\nConectando na rede: ");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  configTime(-10800, 0, "pool.ntp.org");
  obterClima();

  desenharRostoGato();
  if (climaCarregado) {
    mostrarTemperatura(ultimaTemp, ultimaUmid);
  }
}

void loop() {
  unsigned long tempoAtual = millis();

  // --- BOTÃO E BUZZER ---
  bool estadoAtualBotao = digitalRead(PINO_BOTAO);
  
  if (estadoAtualBotao == LOW && estadoAnteriorBotao == HIGH) {
    delay(50); // Debounce
    if (digitalRead(PINO_BOTAO) == LOW) {
      
      bipar(); 
      modoRelogio = !modoRelogio; 
      limparTela(); 
      
      if (!modoRelogio) {
        desenharRostoGato();
        if (climaCarregado) {
          mostrarTemperatura(ultimaTemp, ultimaUmid);
        }
      }
    }
  }
  estadoAnteriorBotao = estadoAtualBotao;

  // --- ATUALIZAÇÃO DO CLIMA ---
  if (tempoAtual - tempoUltimaConsulta >= intervaloConsulta) {
    tempoUltimaConsulta = tempoAtual; 
    obterClima(); 
  }

  // --- ATUALIZAÇÃO DO RELÓGIO DIGITAL ---
  static unsigned long ultimoSegundo = 0;
  if (tempoAtual - ultimoSegundo >= 1000) {
    ultimoSegundo = tempoAtual;

    struct tm timeinfo;
    if (getLocalTime(&timeinfo)) {
      if (modoRelogio) {
        desenharRelogioDigital(timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);
      }
    }
  }
}