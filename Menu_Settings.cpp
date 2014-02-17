#include <Arduino.h>
#include <stdio.h>
#include <Adafruit_GFX.h>
#include "Menu_Settings.h"
#include "Menu_Clockface.h"

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
  ITEM_BACK,

  ITEM_MAX,
};

SettingsMenu::SettingsMenu()
: selection(0)
{}
bool SettingsMenu::update() {return false;}
void SettingsMenu::onEnter() {}
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

// ---- Settings24hMenu ----
Settings24hMenu::Settings24hMenu() {}
bool Settings24hMenu::update() {return false;}
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
  display.setCursor(40, 20);
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

// ---- SettingsTimeMenu ----
SettingsTimeMenu::SettingsTimeMenu()
: last_check(0)
, selection(0)
{}
bool SettingsTimeMenu::update() {
  if (millis() - last_check > 1000) {
    last_check = millis();
    now = RTC.now();
    return true;
  }
  return false;
}
void SettingsTimeMenu::onEnter() {
  last_check = millis();
  now = RTC.now();
  selection = 0;
}
void SettingsTimeMenu::button1() {
  selection = (selection + 1) % 5;
}
void SettingsTimeMenu::button2() {
  if (selection == 4) {
    switchMenu(MENU_SETTINGS);
  } else {
    uint8_t hour = now.hour();
    uint8_t minute = now.minute();
    if (selection == 0) {
      hour = (hour + 1) % 24;
    } else if (selection == 1) {
      minute = (minute + 1) % 60;
    } else if (selection == 2) {
      hour = hour == 0 ? 23 : (hour - 1);
    } else if (selection == 3) {
      minute = minute == 0 ? 59 : (minute - 1);
    }
    now = DateTime(now.year(), now.month(), now.day(), hour, minute, 0);
    last_check = millis();
    RTC.adjust(now);
  }
}
void SettingsTimeMenu::draw(Adafruit_GFX& display) const {
  char buff[9];
  display.setTextColor(WHITE);
  display.setTextSize(2);
  display.setCursor(20, 20);
  sprintf(buff, "%02d:%02d.%02d", now.hour(), now.minute(), now.second());
  display.print(buff);

  if (selection < 2) {
    display.fillTriangle(
      30 + (selection * 35), 5,
      35 + (selection * 35), 10,
      25 + (selection * 35), 10,
      WHITE);
  } else if (selection < 4) {
    display.fillTriangle(
      30 + ((selection - 2) * 35), 45,
      35 + ((selection - 2) * 35), 40,
      25 + ((selection - 2) * 35), 40,
      WHITE);
  }
  if (selection == 4) {
    display.fillRect(0, 55, WIDTH, HEIGHT, WHITE);
    display.setTextColor(BLACK, WHITE);
  } else {
    display.setTextColor(WHITE);
  }
  display.setTextSize(1);
  display.setCursor(5, 56);
  display.print("Back");
}

