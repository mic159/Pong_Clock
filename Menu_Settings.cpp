#include <Arduino.h>
#include <Adafruit_GFX.h>
#include "Menu_Settings.h"
#include "Menu_Clockface.h"

#define BLACK 0
#define WHITE 1

enum Items {
  ITEM_24H,
  ITEM_TIME,
  ITEM_BACK,

  ITEM_MAX,
};

SettingsMenu::SettingsMenu()
: selection(0)
{}

void SettingsMenu::update() {}

void SettingsMenu::onEnter() {}

void SettingsMenu::button1() {
  selection = (selection + 1) % ITEM_MAX;
}

void SettingsMenu::button2() {
  if (selection == ITEM_BACK) {
    selection = 0;
    switchMenu(MENU_CLOCK);
  } else if (selection == ITEM_24H) {
    switchMenu(MENU_SETTINGS_24H);
  }
}

void SettingsMenu::draw(Adafruit_GFX& display) const {
  display.setCursor(0, 0);
  display.setTextSize(1);
  for (uint8_t i = 0; i < ITEM_MAX; ++i) {
    if (selection == i) {
      display.setTextColor(BLACK, WHITE);
    } else {
      display.setTextColor(WHITE);
    }
    switch (i) {
    case ITEM_24H:
      display.println("Set 12/24h mode");
      break;
    case ITEM_TIME:
      display.println("Set Date/Time");
      break;
    case ITEM_BACK:
      display.println("Back");
      break;
    }
  }
}

Settings24hMenu::Settings24hMenu() {}
void Settings24hMenu::update() {}
void Settings24hMenu::onEnter() {}
void Settings24hMenu::button1() {
  bool state = static_cast<ClockFaceMenu*>(getMenu(MENU_CLOCK))->mode24h;
  static_cast<ClockFaceMenu*>(getMenu(MENU_CLOCK))->mode24h = !state;
}
void Settings24hMenu::button2() {
  switchMenu(MENU_SETTINGS);
}
void Settings24hMenu::draw(Adafruit_GFX& display) const {
  bool state = static_cast<ClockFaceMenu*>(getMenu(MENU_CLOCK))->mode24h;
  display.setTextColor(WHITE);
  display.setTextSize(3);
  display.setCursor(5, 20);
  if (state) {
    display.print("12hr");
  } else {
    display.print("24hr");
  }
  display.fillTriangle(
    (128 / 2)    , 5, 
    (128 / 2) + 5, 10,
    (128 / 2) - 5, 10,
    WHITE);
  display.fillTriangle(
    (128 / 2)    , 60, 
    (128 / 2) + 5, 55,
    (128 / 2) - 5, 55,
    WHITE);
}

