#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "Menu_Settings_Brightness.h"
#include "State.h"

// Some graphics constants
#define BLACK 0
#define WHITE 1
#define WIDTH 128
#define HEIGHT 64

extern Adafruit_SSD1306 display;

enum MenuItems {
  ITEM_100,
  ITEM_50,
  ITEM_25,
  ITEM_BACK,
  ITEM_MAX
};

SettingsBrightnessMenu::SettingsBrightnessMenu()
: selection(0)
{}

bool SettingsBrightnessMenu::update() {
  return state.timeMinuteUpdated;
}

void SettingsBrightnessMenu::onEnter() {
}

void SettingsBrightnessMenu::button1() {
  selection = (selection + 1) % ITEM_MAX;
}

void SettingsBrightnessMenu::button2() {
  if (selection == ITEM_BACK) {
    selection = 0;
    switchMenu(MENU_CLOCK);
  } else if (selection == ITEM_100) {
    display.dim(false);
    state.brightness = 1;
  } else if (selection == ITEM_50) {
    display.dim(true);
    state.brightness = 0;
  } else if (selection == ITEM_25) {
    display.dim(50);
    state.brightness = 0;
  }
}

void SettingsBrightnessMenu::draw(Adafruit_GFX* display) const {
  char buff[9];
  // Border
  display->drawRect(0, 0, WIDTH, HEIGHT, WHITE);

  // Title
  display->setTextColor(WHITE);
  display->setTextSize(1);
  display->setCursor(2, 2);
  display->print(F("Set Date"));
  display->drawFastVLine(50, 0, 10, WHITE);
  display->drawFastHLine(0, 10, 50, WHITE);

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

  // Menu Items
  for (uint8_t i = 0; i < ITEM_MAX; ++i) {
    display->setCursor(5, 13 + (i * 10));
    if (selection == i) {
      display->setTextColor(BLACK, WHITE);
      display->fillRect(
        0    , 12 + (i * 10),
        WIDTH, 9,
        WHITE);
    } else {
      display->setTextColor(WHITE);
    }
    switch (i) {
    case ITEM_100:
      display->print(F("100%"));
      break;
    case ITEM_50:
      display->print(F("50%"));
      break;
    case ITEM_25:
      display->print(F("25%"));
      break;
    case ITEM_BACK:
      display->print(F("Back"));
      break;
    }
  }
}

