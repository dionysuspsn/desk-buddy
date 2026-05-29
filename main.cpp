#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <time.h>
#include "secrets.h"
#include "Tela.h"

#define PINO_BOTAO 15 // Pino digital onde o botão está ligado

// Link da API usando a chave definida no secrets.h
String openWeatherUrl = "http://api.openweathermap.org/data/2.5/weather?q=Fortaleza,BR&units=metric&appid=" + String(OPENWEATHER_API_KEY);

unsigned long tempoUltimaConsulta = 0; 
const long intervaloConsulta = 600000; // 10 minutos em milissegundos

// Variáveis para guardar o clima na memória
float ultimaTemp = 0.0;
int ultimaUmid = 0;
bool climaCarregado = false;

// Controlo de Ecrãs (Modo Gato = false | Modo Relógio = true)
bool modoRelogio = false; 
bool estadoAnteriorBotao = HIGH;

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
        
        // Só desenha a temperatura se o ecrã do gato estiver ativo
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

  // Configura o pino do botão usando o resistor interno do ESP32
  pinMode(PINO_BOTAO, INPUT_PULLUP);

  // Liga a luz do ecrã (Pino 32)
  pinMode(32, OUTPUT);       
  digitalWrite(32, HIGH);    

  // Inicialização Base do Ecrã
  inicializarTela();

  Serial.print("\nA ligar à rede: ");
  Serial.println(WIFI_SSID);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  // Puxa a hora da internet (Fuso horário UTC-3)
  configTime(-10800, 0, "pool.ntp.org");

  // Puxa o clima logo ao ligar
  obterClima();

  // Inicia sempre no Modo Gato
  desenharRostoGato();
  if (climaCarregado) {
    mostrarTemperatura(ultimaTemp, ultimaUmid);
  }
}

void loop() {
  unsigned long tempoAtual = millis();

  // --- 1. LÓGICA DO BOTÃO (TROCA DE ECRÃ) ---
  bool estadoAtualBotao = digitalRead(PINO_BOTAO);
  
  if (estadoAtualBotao == LOW && estadoAnteriorBotao == HIGH) {
    delay(50); // Filtro "Debounce" para ignorar ruído elétrico
    if (digitalRead(PINO_BOTAO) == LOW) {
      modoRelogio = !modoRelogio; // Inverte o ecrã atual
      
      limparTela(); // Apaga tudo
      
      if (modoRelogio) {
        // Foi para o modo relógio
        inicializarRelogio();
      } else {
        // Voltou para o modo gato
        desenharRostoGato();
        if (climaCarregado) {
          mostrarTemperatura(ultimaTemp, ultimaUmid);
        }
      }
    }
  }
  estadoAnteriorBotao = estadoAtualBotao;

  // --- 2. CRONÓMETRO DO CLIMA (A cada 10 min) ---
  if (tempoAtual - tempoUltimaConsulta >= intervaloConsulta) {
    tempoUltimaConsulta = tempoAtual; 
    obterClima(); 
  }

  // --- 3. CRONÓMETRO DO RELÓGIO (A cada 1 seg) ---
  static unsigned long ultimoSegundo = 0;
  if (tempoAtual - ultimoSegundo >= 1000) {
    ultimoSegundo = tempoAtual;

    struct tm timeinfo;
    if (getLocalTime(&timeinfo)) {
      if (modoRelogio) {
        atualizarRelogio(timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);
      }
    }
  }
}