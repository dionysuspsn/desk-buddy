#include <Arduino.h>
#include "SPI.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"

#define TFT_CS   5
#define TFT_DC   4
#define TFT_MOSI 23
#define TFT_SCK  18
#define TFT_RST  22

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);

// função para interpolar cores (gradiente)
uint16_t gradientColor(float t) {
  // cor interna (#094601)
  int r1 = 9,  g1 = 70, b1 = 1;

  // cor externa (#008009)
  int r2 = 0,  g2 = 128, b2 = 9;

  // interpolação
  int r = r1 + (r2 - r1) * t;
  int g = g1 + (g2 - g1) * t;
  int b = b1 + (b2 - b1) * t;

  return tft.color565(r, g, b);
}

// desenha gradiente radial
void drawGradientCircle(int cx, int cy, int radius) {
  for (int r = radius; r > 0; r--) {
    float t = (float)r / radius; // 0 → centro, 1 → borda
    uint16_t color = gradientColor(t);
    tft.fillCircle(cx, cy, r, color);
  }
}

void drawEye(int cx, int cy) {
  // fundo com gradiente
  drawGradientCircle(cx, cy, 58);

  // contorno
  tft.drawCircle(cx, cy, 60, ILI9341_WHITE);

  // pupila vertical
  tft.fillRoundRect(cx - 5, cy - 35, 10, 70, 5, ILI9341_BLACK);

  // brilho
  tft.fillCircle(cx + 15, cy - 15, 8, ILI9341_WHITE);
}

// NOVA FUNÇÃO: Desenha focinho e boca
// (x, y) é a posição central do nariz
void drawNoseAndMouth(int x, int y) {
  // 1. O Nariz (ponta): um pequeno círculo branco
  tft.fillCircle(x, y, 12, tft.color565(255, 179, 204));

  // 2. O Filtro (linha vertical descendo do nariz)
  int filterLen = 24; 
  tft.drawLine(x, y, x, y + filterLen, ILI9341_WHITE);

  // 3. A Boca (biquinho)
  int bocaWidth = 40; // largura de cada lado da boca
  int bocaDepth = 20; // profundidade da curva
  int baseFilterY = y + filterLen;

  // Lado esquerdo da boca 
  tft.drawLine(x, baseFilterY, x - bocaWidth / 2, baseFilterY + bocaDepth / 2, ILI9341_WHITE);
  tft.drawLine(x - bocaWidth / 2, baseFilterY + bocaDepth / 2, x - bocaWidth, baseFilterY, ILI9341_WHITE);

  // Lado direito da boca 
  tft.drawLine(x, baseFilterY, x + bocaWidth / 2, baseFilterY + bocaDepth / 2, ILI9341_WHITE);
  tft.drawLine(x + bocaWidth / 2, baseFilterY + bocaDepth / 2, x + bocaWidth, baseFilterY, ILI9341_WHITE);
}

void setup() {
  tft.begin();
  tft.setRotation(1);
  tft.fillScreen(ILI9341_BLACK);

  // Desenha os olhos
  drawEye(60, 90);
  drawEye(240, 90);

  // Desenha o focinho posicionado matematicamente
  drawNoseAndMouth(150, 170);
}

void loop() {}