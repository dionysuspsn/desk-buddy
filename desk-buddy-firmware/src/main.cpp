#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <time.h> // Biblioteca necessária para o relógio
#include "secrets.h"
#include "Tela.h"

// Monta o link da API usando a chave secreta
String openWeatherUrl = "http://api.openweathermap.org/data/2.5/weather?q=Fortaleza,BR&units=metric&appid=" + String(OPENWEATHER_API_KEY);

// Variáveis para o cronômetro do clima (atualizar a cada 10 min)
unsigned long tempoUltimaConsulta = 0; 
const long intervaloConsulta = 600000; // 600.000 ms = 10 minutos

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
        
        // Manda os dados para a tela TFT 
        mostrarTemperatura(temperatura, umidade);
        
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
  delay(1000); // Dá um tempo para o Monitor Serial estabilizar

  // --- PARTE EXTRA DA OPÇÃO 2 (LIGA A LUZ DA TELA) ---
  pinMode(32, OUTPUT);       
  digitalWrite(32, HIGH);    
  // --------------------------------------------------

  // 1. Inicializa o hardware da tela, desenha o gato e a base do relógio
  inicializarTela();
  desenharRostoGato();
  inicializarRelogio();

  // 2. Conecta ao Wi-Fi
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

  // 3. Configura a hora direto da internet.
  // Fuso horário UTC-3 (são -10800 segundos em relação a Londres)
  configTime(-10800, 0, "pool.ntp.org");

  // 4. Puxa a temperatura pela primeira vez logo após conectar
  obterClima();
}

void loop() {
  // Pega o tempo atual do relógio interno do ESP32
  unsigned long tempoAtual = millis();

  // --- CRONÓMETRO DO CLIMA (A cada 10 minutos) ---
  if (tempoAtual - tempoUltimaConsulta >= intervaloConsulta) {
    tempoUltimaConsulta = tempoAtual; // Reseta o cronômetro
    obterClima(); // Busca e desenha a nova temperatura
  }

  // --- CRONÓMETRO DO RELÓGIO (A cada 1 segundo) ---
  static unsigned long ultimoSegundo = 0;
  if (tempoAtual - ultimoSegundo >= 1000) {
    ultimoSegundo = tempoAtual;

    // Pega a hora da placa e envia para a tela atualizar os ponteiros
    struct tm timeinfo;
    if (getLocalTime(&timeinfo)) {
      atualizarRelogio(timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);
    }
  }
}