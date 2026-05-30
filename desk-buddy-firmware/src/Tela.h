#pragma once
#include <Arduino.h>

// Declaração dos pinos da tela
#define TFT_CS   5
#define TFT_DC   4
#define TFT_MOSI 23
#define TFT_SCK  18
#define TFT_RST  22 

// Funções do sistema de tela
void inicializarTela();
void limparTela();
void desenharRostoGato();
void mostrarTemperatura(float temp, int umidade);
void desenharRelogioDigital(int h, int m, int s);