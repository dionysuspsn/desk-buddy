#pragma once
#include <Arduino.h>

// Declaração dos pinos da tela
#define TFT_CS   5
#define TFT_DC   4
#define TFT_MOSI 23
#define TFT_SCK  18
#define TFT_RST  22 

// Declaração das funções para que o main as conheça
void inicializarTela();
void desenharRostoGato();
void mostrarTemperatura(float temp, int umidade);
void inicializarRelogio();
void atualizarRelogio(int h, int m, int s);