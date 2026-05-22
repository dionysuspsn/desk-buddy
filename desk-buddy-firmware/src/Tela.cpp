#include <math.h> // Adicione isto no topo
#include "Tela.h"
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
  // 1. Limpa o topo da tela com um retângulo preto (para não embaralhar os números quando atualizar)
  tft.fillRect(0, 0, 320, 30, ILI9341_BLACK);

  // 2. Configura a fonte (cor branca, tamanho 2)
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(2);
  
  // 3. Escreve a Temperatura no canto superior esquerdo
  tft.setCursor(10, 10);
  tft.print("Temp: ");
  tft.print(temp, 1); // O '1' é para mostrar só uma casa decimal
  tft.print(" C");

  // 4. Escreve a Umidade no canto superior direito
  tft.setCursor(180, 10);
  tft.print("Umid: ");
  tft.print(umidade);
  tft.print(" %");
}
// Posição e tamanho do relógio
const int RELOGIO_CX = 160;
const int RELOGIO_CY = 180; 
const int RELOGIO_R = 45;

// Memória para apagar os ponteiros antigos (começam no centro)
int last_sx = RELOGIO_CX, last_sy = RELOGIO_CY;
int last_mx = RELOGIO_CX, last_my = RELOGIO_CY;
int last_hx = RELOGIO_CX, last_hy = RELOGIO_CY;

void inicializarRelogio() {
  // Desenha o círculo exterior do relógio
  tft.drawCircle(RELOGIO_CX, RELOGIO_CY, RELOGIO_R, ILI9341_WHITE);
  tft.drawCircle(RELOGIO_CX, RELOGIO_CY, RELOGIO_R + 1, ILI9341_WHITE);
  // Ponto central
  tft.fillCircle(RELOGIO_CX, RELOGIO_CY, 3, ILI9341_WHITE);
}

void atualizarRelogio(int h, int m, int s) {
  // 1. Converte o tempo para ângulos (Subtraímos 90 graus para o 0 começar no topo/12h)
  float s_angle = (s * 6 - 90) * 0.0174533; // 0.0174533 é Pi/180 para converter pra radianos
  float m_angle = (m * 6 - 90) * 0.0174533;
  float h_angle = ((h % 12) * 30 + (m * 0.5) - 90) * 0.0174533;

  // 2. Calcula onde fica a ponta de cada ponteiro
  int sx = RELOGIO_CX + (RELOGIO_R - 5) * cos(s_angle);
  int sy = RELOGIO_CY + (RELOGIO_R - 5) * sin(s_angle);

  int mx = RELOGIO_CX + (RELOGIO_R - 10) * cos(m_angle);
  int my = RELOGIO_CY + (RELOGIO_R - 10) * sin(m_angle);

  int hx = RELOGIO_CX + (RELOGIO_R - 20) * cos(h_angle);
  int hy = RELOGIO_CY + (RELOGIO_R - 20) * sin(h_angle);

  // 3. APAGA os ponteiros velhos desenhando-os de PRETO
  tft.drawLine(RELOGIO_CX, RELOGIO_CY, last_sx, last_sy, ILI9341_BLACK);
  tft.drawLine(RELOGIO_CX, RELOGIO_CY, last_mx, last_my, ILI9341_BLACK);
  tft.drawLine(RELOGIO_CX, RELOGIO_CY, last_hx, last_hy, ILI9341_BLACK);

  // 4. DESENHA os novos ponteiros coloridos
  tft.drawLine(RELOGIO_CX, RELOGIO_CY, hx, hy, ILI9341_WHITE); // Hora
  tft.drawLine(RELOGIO_CX, RELOGIO_CY, mx, my, ILI9341_CYAN);  // Minuto
  tft.drawLine(RELOGIO_CX, RELOGIO_CY, sx, sy, ILI9341_RED);   // Segundo

  // 5. Refaz o pino central para os ponteiros não o cortarem
  tft.fillCircle(RELOGIO_CX, RELOGIO_CY, 3, ILI9341_WHITE);

  // 6. Atualiza a memória para o próximo segundo
  last_sx = sx; last_sy = sy;
  last_mx = mx; last_my = my;
  last_hx = hx; last_hy = hy;
}