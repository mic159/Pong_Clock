/*
 * Pacman Clockface
 * A ghost is chasing Pacman accross the screen
 */

#include <Arduino.h>
#include <Adafruit_GFX.h>
#include "Clockface_Pacman.h"
#include "State.h"

#define WHITE 1
#define BLACK 0
#define WIDTH 128
#define HEIGHT 64

uint8_t PROGMEM pacman_bmp[13*2*2] = {
  B00000011, B11100000,
  B00001111, B11111000,
  B00011111, B11111100,
  B00011111, B11111100,
  B00111111, B11110000,
  B00111111, B10000000,
  B00111110, B00000000,
  B00111111, B10000000,
  B00111111, B11110000,
  B00011111, B11111100,
  B00011111, B11111100,
  B00001111, B11111000,
  B00000011, B11100000,

  B00000011, B11100000,
  B00001111, B11100000,
  B00011111, B11100000,
  B00011111, B11000000,
  B00111111, B10000000,
  B00111111, B00000000,
  B00111110, B00000000,
  B00111111, B00000000,
  B00111111, B10000000,
  B00011111, B11000000,
  B00011111, B11100000,
  B00001111, B11100000,
  B00000011, B11100000,
};

uint8_t PROGMEM blinky_bmp[28*2] = {
  B00000011, B11000000,
  B00001111, B11110000,
  B00011111, B11111000,
  B00111111, B11111100,
  B00111001, B11100100,
  B00110000, B11000000,
  B01110011, B11001110,
  B01110011, B11001110,
  B01111001, B11100110,
  B01111111, B11111110,
  B01111111, B11111110,
  B01111111, B11111110,
  B01111011, B11011110,
  B00110001, B10001100,

  B00000011, B11000000,
  B00001111, B11110000,
  B00011111, B11111000,
  B00111111, B11111100,
  B00111001, B11100100,
  B00110000, B11000000,
  B01110011, B11001110,
  B01110011, B11001110,
  B01111001, B11100110,
  B01111111, B11111110,
  B01111111, B11111110,
  B01111111, B11111110,
  B01101110, B01110110,
  B01000110, B01100010,
};

ClockfacePacman::ClockfacePacman()
: x(0)
{}

void ClockfacePacman::update(uint8_t hour, uint8_t minute) {
  x = ((x + 16 + 2) % (WIDTH + 41)) - 16;
}

void ClockfacePacman::draw(Adafruit_GFX* display) const {
  char buff[9];

  uint8_t hour = state.now.hour();
  if (!state.mode24h) {
    hour = hour % 12;
    if (hour == 0) hour = 12;
  }
  snprintf_P(buff, 9, PSTR("%02u:%02u")
    , hour
    , state.now.minute()
    );

  // Hour:Minute
  display->setTextColor(WHITE);
  display->setTextSize(2);
  display->setCursor(10, 18);
  display->print(buff);

  // am/pm suffix
  display->setTextSize(1);
  if (!state.mode24h) {
    display->setCursor(70, 25);
    if (state.now.hour() > 11) {
      display->print(F("pm"));
    } else {
      display->print(F("am"));
    }
  }

  // Pacman!
  bool alt = (x+16) % 20 < 10;
  display->drawBitmap(x, HEIGHT - 20, pacman_bmp + (alt ? 0:26), 16, 13, WHITE);
  display->drawBitmap(x - 25, HEIGHT - 20, blinky_bmp + (alt ? 0:28), 16, 14, WHITE);
}

