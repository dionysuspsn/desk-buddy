#include <Arduino.h>
#include <WiFi.h>
#include "secrets.h" // Chama o ficheiro das passwords

void setup() {
  // Inicia a comunicação com o computador
  Serial.begin(115200);
  delay(1000); 

  Serial.println("\n--- Desk Buddy: A iniciar sistema de rede ---");
  Serial.print("Tentando conectar-se com a rede...");
  Serial.println(WIFI_SSID);

  // Comando para iniciar a ligação Wi-Fi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  // Fica num ciclo de espera até ligar
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  // Quando sair do ciclo o Wi-Fi liga
  Serial.println("\nSucesso! O ESP32 está online.");
  Serial.print("Endereço IP: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  // O loop fica vazio por agora, estamos apenas a testar a ligação no setup
}