#include <Arduino.h>
#include <Adafruit_GFX.h>
#include "Menu_Settings.h"
#include "State.h"

// Grab RTC instance from .ino
extern RTC_DS1307 RTC;

// Some graphics constants
#define BLACK 0
#define WHITE 1
#define WIDTH 128
#define HEIGHT 64

// ---- SettingsMenu ----
// Menu items for SettingsMenu
enum Items {
  ITEM_24H,
  ITEM_TIME,
  ITEM_DATE,
  ITEM_BRIGHT,
  ITEM_BACK,

  ITEM_MAX,
};

SettingsMenu::SettingsMenu()
: selection(0)
{}

bool SettingsMenu::update() {
  return state.timeMinuteUpdated;
}

void SettingsMenu::button1() {
  selection = (selection + 1) % ITEM_MAX;
}

void SettingsMenu::button2() {
  if (selection == ITEM_BACK) {
    selection = 0;
    switchMenu(MENU_CLOCK);
  } else if (selection == ITEM_TIME) {
    switchMenu(MENU_SETTINGS_TIME);
  } else if (selection == ITEM_24H) {
    switchMenu(MENU_SETTINGS_24H);
  } else if (selection == ITEM_DATE) {
    switchMenu(MENU_SETTINGS_DATE);
  } else if (selection == ITEM_BRIGHT) {
    switchMenu(MENU_SETTINGS_BRIGHTNESS);
  }
}

void SettingsMenu::draw(Adafruit_GFX* display) const {
  drawLayout(display, F("Settings"));

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
    case ITEM_24H:
      display->print(F("Set 12/24h mode"));
      break;
    case ITEM_TIME:
      display->print(F("Set Time"));
      break;
    case ITEM_DATE:
      display->print(F("Set Date"));
      break;
    case ITEM_BRIGHT:
      display->print(F("Set Brightness"));
      break;
    case ITEM_BACK:
      display->print(F("Back"));
      break;
    }
  }
}

