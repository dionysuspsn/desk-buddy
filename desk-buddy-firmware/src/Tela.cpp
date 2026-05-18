#include "tela.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);

uint16_t gradientColor(float t) {
  int r1 = 9,  g1 = 70, b1 = 1;
  int r2 = 0,  g2 = 128, b2 = 9;
  int r = r1 + (r2 - r1) * t;
  int g = g1 + (g2 - g1) * t;
  int b = b1 + (b2 - b1) * t;
  return tft.color565(r, g, b);
}

void drawGradientCircle(int cx, int cy, int radius) {
  for (int r = radius; r > 0; r--) {
    float t = (float)r / radius;
    uint16_t color = gradientColor(t);
    tft.fillCircle(cx, cy, r, color);
  }
}

void drawEye(int cx, int cy) {
  drawGradientCircle(cx, cy, 58);
  tft.drawCircle(cx, cy, 60, ILI9341_WHITE);
  tft.fillRoundRect(cx - 5, cy - 35, 10, 70, 5, ILI9341_BLACK);
  tft.fillCircle(cx + 15, cy - 15, 8, ILI9341_WHITE);
}

void drawNoseAndMouth(int x, int y) {
  tft.fillCircle(x, y, 12, tft.color565(255, 179, 204));
  int filterLen = 24; 
  tft.drawLine(x, y, x, y + filterLen, ILI9341_WHITE);
  int bocaWidth = 40;
  int bocaDepth = 20;
  int baseFilterY = y + filterLen;
  tft.drawLine(x, baseFilterY, x - bocaWidth / 2, baseFilterY + bocaDepth / 2, ILI9341_WHITE);
  tft.drawLine(x - bocaWidth / 2, baseFilterY + bocaDepth / 2, x - bocaWidth, baseFilterY, ILI9341_WHITE);
  tft.drawLine(x, baseFilterY, x + bocaWidth / 2, baseFilterY + bocaDepth / 2, ILI9341_WHITE);
  tft.drawLine(x + bocaWidth / 2, baseFilterY + bocaDepth / 2, x + bocaWidth, baseFilterY, ILI9341_WHITE);
}

void inicializarTela() {
  tft.begin();
  tft.setRotation(1);
  tft.fillScreen(ILI9341_BLACK);
}

void desenharRostoGato() {
  drawEye(60, 90);
  drawEye(240, 90);
  drawNoseAndMouth(150, 170);
}

void mostrarTemperatura(float temp, int umidade) {
  // Próxima aula: lógica para desenhar o texto da temperatura na tela
}