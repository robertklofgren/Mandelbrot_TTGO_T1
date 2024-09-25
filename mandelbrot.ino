#include <TFT_eSPI.h>
#include <SPI.h>
#include <cmath>

TFT_eSPI tft = TFT_eSPI();

#define SCREEN_WIDTH  135
#define SCREEN_HEIGHT 240

#define BUTTON_1  35
#define BUTTON_2  0

double minRe = -2.0;
double maxRe = 1.0;
double minIm;
double maxIm;
int MAX_ITER = 100;

double zoomFactor = 0.8;
int zoomLevel = 0;
double panFactor = 0.2;

double reCenter;
double imCenter;

bool needsRedraw = true;
int selectedOption = 0;
String menuOptions[] = { "Zoom In", "Zoom Out", "Reset View", "Pan Left", "Pan Right", "Pan Up", "Pan Down" };
const int numOptions = 7;

void setup() {
  Serial.begin(115200);

  tft.init();
  tft.setRotation(0);
  tft.fillScreen(TFT_BLACK);

  pinMode(BUTTON_1, INPUT_PULLUP);
  pinMode(BUTTON_2, INPUT_PULLUP);

  minIm = -(maxRe - minRe) * SCREEN_HEIGHT / SCREEN_WIDTH / 2.0;
  maxIm = -minIm;

  reCenter = (minRe + maxRe) / 2.0;
  imCenter = (minIm + maxIm) / 2.0;

  drawMandelbrot();
  displayMenu();
}

void loop() {
  bool button1Pressed = digitalRead(BUTTON_1) == LOW;
  bool button2Pressed = digitalRead(BUTTON_2) == LOW;

  if (button1Pressed) {
    cycleMenu();
    delay(200);
  } else if (button2Pressed) {
    executeOption();
    delay(200);
  }

  if (needsRedraw) {
    drawMandelbrot();
    needsRedraw = false;
    displayMenu();
  }
}

void drawMandelbrot() {
  tft.startWrite();

  maxIm = minIm + (maxRe - minRe) * SCREEN_HEIGHT / SCREEN_WIDTH;

  double reFactor = (maxRe - minRe) / (SCREEN_WIDTH - 1);
  double imFactor = (maxIm - minIm) / (SCREEN_HEIGHT - 1);

  for (int y = 0; y < SCREEN_HEIGHT - 20; y++) {
    double c_im = maxIm - y * imFactor;

    for (int x = 0; x < SCREEN_WIDTH; x++) {
      double c_re = minRe + x * reFactor;

      double Z_re = c_re, Z_im = c_im;
      bool isInside = true;
      int n;

      for (n = 0; n < MAX_ITER; n++) {
        double Z_re2 = Z_re * Z_re;
        double Z_im2 = Z_im * Z_im;

        if (Z_re2 + Z_im2 > 4) {
          isInside = false;
          break;
        }

        Z_im = 2 * Z_re * Z_im + c_im;
        Z_re = Z_re2 - Z_im2 + c_re;
      }

      uint16_t color;
      if (isInside) {
        color = TFT_BLACK;
      } else {
        color = tft.color565(n % 16 * 16, n % 32 * 8, n % 8 * 32);
      }

      tft.drawPixel(x, y, color);
    }
  }

  tft.endWrite();
}

void zoomIn() {
  zoomFactor *= 0.9;
  double reWidth = (maxRe - minRe) * zoomFactor;
  double imHeight = (maxIm - minIm) * zoomFactor;

  minRe = reCenter - reWidth / 2.0;
  maxRe = reCenter + reWidth / 2.0;
  minIm = imCenter - imHeight / 2.0;
  maxIm = imCenter + imHeight / 2.0;

  zoomLevel++;
  MAX_ITER += 20;

  needsRedraw = true;
}

void zoomOut() {
  zoomFactor *= 1.1;
  double reWidth = (maxRe - minRe) / zoomFactor;
  double imHeight = (maxIm - minIm) / zoomFactor;

  minRe = reCenter - reWidth / 2.0;
  maxRe = reCenter + reWidth / 2.0;
  minIm = imCenter - imHeight / 2.0;
  maxIm = imCenter + imHeight / 2.0;

  zoomLevel--;
  MAX_ITER -= 20;

  needsRedraw = true;
}

void resetView() {
  minRe = -2.0;
  maxRe = 1.0;
  minIm = -(maxRe - minRe) * SCREEN_HEIGHT / SCREEN_WIDTH / 2.0;
  maxIm = -minIm;

  zoomLevel = 0;
  zoomFactor = 0.8;
  MAX_ITER = 100;

  reCenter = (minRe + maxRe) / 2.0;
  imCenter = (minIm + maxIm) / 2.0;

  needsRedraw = true;
}

void pan(double deltaX, double deltaY) {
  double reWidth = maxRe - minRe;
  double imHeight = maxIm - minIm;

  minRe += deltaX * reWidth;
  maxRe += deltaX * reWidth;
  minIm += deltaY * imHeight;
  maxIm += deltaY * imHeight;

  reCenter = (minRe + maxRe) / 2.0;
  imCenter = (minIm + maxIm) / 2.0;

  needsRedraw = true;
}

void cycleMenu() {
  selectedOption = (selectedOption + 1) % numOptions;
  displayMenu();
}

void executeOption() {
  switch (selectedOption) {
    case 0: zoomIn(); break;
    case 1: zoomOut(); break;
    case 2: resetView(); break;
    case 3: pan(-0.2, 0); break;
    case 4: pan(0.2, 0); break;
    case 5: pan(0, 0.2); break;
    case 6: pan(0, -0.2); break;
  }
}

void displayMenu() {
  tft.fillRect(0, SCREEN_HEIGHT - 20, SCREEN_WIDTH, 20, TFT_BLACK);
  tft.setCursor(5, SCREEN_HEIGHT - 15);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextSize(1);
  tft.print("Option: ");
  tft.print(menuOptions[selectedOption]);
}
