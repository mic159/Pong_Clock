#include <Arduino.h>
#include <Adafruit_GFX.h>
#include "Menu_Settings_24.h"
#include "State.h"

// Some graphics constants
#define BLACK 0
#define WHITE 1
#define WIDTH 128
#define HEIGHT 64

Settings24hMenu::Settings24hMenu() {}

bool Settings24hMenu::update()  {
  return state.timeMinuteUpdated;
}

void Settings24hMenu::onEnter() {}

void Settings24hMenu::button1() {
  state.mode24h = !state.mode24h;
  state.save();
}

void Settings24hMenu::button2() {
  switchMenu(MENU_SETTINGS);
}

void Settings24hMenu::draw(Adafruit_GFX* display) const {
  char buff[6];
  // Border
  display->drawRect(0, 0, WIDTH, HEIGHT, WHITE);

  // Title
  display->setTextColor(WHITE);
  display->setTextSize(1);
  display->setCursor(2, 2);
  display->print(F("12/24hr mode"));
  display->drawFastVLine(74, 0, 10, WHITE);
  display->drawFastHLine(0, 10, 74, WHITE);

  // Clock
  display->fillRect(
    WIDTH - 33, 0,
    33, 10,
    WHITE);
  display->drawFastHLine(WIDTH - 32, 10, 32, WHITE);
  display->setTextColor(BLACK, WHITE);
  display->setCursor(WIDTH - 31, 2);
  snprintf_P(buff, 6, PSTR("%02d:%02d"), state.now.hour(), state.now.minute());
  display->print(buff);

  // Selector
  display->setTextColor(WHITE);
  display->setTextSize(2);
  display->setCursor(40, 23);
  if (state.mode24h) {
    display->print(F("24hr"));
  } else {
    display->print(F("12hr"));
  }
  display->fillTriangle(
    (WIDTH / 2)    , 15,
    (WIDTH / 2) + 5, 20,
    (WIDTH / 2) - 5, 20,
    WHITE);
  display->fillTriangle(
    (WIDTH / 2)    , 45,
    (WIDTH / 2) + 5, 40,
    (WIDTH / 2) - 5, 40,
    WHITE);
}
