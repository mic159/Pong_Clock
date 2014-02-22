#include <Arduino.h>
#include <Adafruit_GFX.h>
#include "Menu_Settings_Time.h"
#include "State.h"

// Grab RTC instance from .ino
extern RTC_DS1307 RTC;

// Some graphics constants
#define BLACK 0
#define WHITE 1
#define WIDTH 128
#define HEIGHT 64

void Menu::drawLayout(Adafruit_GFX* display, const __FlashStringHelper* title) const {
  char buff[8];
  // Border
  display->drawRect(0, 0, WIDTH, HEIGHT, WHITE);

  // Title
  display->setTextColor(WHITE);
  display->setTextSize(1);
  display->setCursor(2, 2);
  uint16_t title_width = display->print(title);
  display->drawFastHLine(0, 10, title_width * 6 + 2, WHITE);
  display->drawFastVLine(title_width * 6 + 2, 0, 10, WHITE);

  // Clock
  uint8_t w = state.mode24h ? 31 : 37;
  display->fillRect(
    WIDTH - (w + 2), 0,
    (w + 2), 10,
    WHITE);
  display->drawFastHLine(WIDTH - (w + 1), 10, (w + 1), WHITE);
  display->setTextColor(BLACK, WHITE);
  if (state.mode24h) {
    display->setCursor(WIDTH - w, 2);
    snprintf_P(buff, 7, PSTR("%02d:%02d"), state.now.hour(), state.now.minute());
  } else {
    display->setCursor(WIDTH - w, 2);
    uint8_t hour = state.now.hour() % 12;
    if (hour == 0) hour = 12;
    snprintf_P(buff, 7, PSTR("%02d:%02d%c"),
      hour,
      state.now.minute(),
      state.now.hour() > 11 ? 'P' : 'A'
      );
  }
  display->print(buff);
}

