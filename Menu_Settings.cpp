#include <Arduino.h>
#include <stdio.h>
#include <Adafruit_GFX.h>
#include "Menu_Settings.h"
#include "Menu_Clockface.h"
#include "State.h"
#include "Util.h"

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

bool SettingsMenu::update() {
  return state.timeMinuteUpdated;
}

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

void SettingsMenu::draw(Adafruit_GFX* display) const {
  char buff[6];
  // Border
  display->drawRect(0, 0, WIDTH, HEIGHT, WHITE);

  // Title
  display->setTextSize(1);
  display->setCursor(2, 2);
  display->setTextColor(WHITE);
  display_print(PSTR("Settings"));
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
  snprintf(buff, 6, "%02d:%02d", state.now.hour(), state.now.minute());
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
    case ITEM_24H:
      display_print(PSTR("Set 12/24h mode"));
      break;
    case ITEM_TIME:
      display_print(PSTR("Set Time"));
      break;
    case ITEM_BACK:
      display_print(PSTR("Back"));
      break;
    }
  }
}

// ---- Settings24hMenu ----
Settings24hMenu::Settings24hMenu() {}

bool Settings24hMenu::update()  {
  return state.timeMinuteUpdated;
}

void Settings24hMenu::onEnter() {}

void Settings24hMenu::button1() {
  state.mode24h = !state.mode24h;
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
  display_print(PSTR("12/24hr mode"));
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
  snprintf(buff, 6, "%02d:%02d", state.now.hour(), state.now.minute());
  display->print(buff);

  // Selector
  display->setTextColor(WHITE);
  display->setTextSize(2);
  display->setCursor(40, 23);
  if (state.mode24h) {
    display_print(PSTR("24hr"));
  } else {
    display_print(PSTR("12hr"));
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

// ---- SettingsTimeMenu ----
SettingsTimeMenu::SettingsTimeMenu()
: selection(0)
{}

bool SettingsTimeMenu::update() {
  return state.timeMinuteUpdated;
}

void SettingsTimeMenu::onEnter() {
  selection = 0;
}

void SettingsTimeMenu::button1() {
  selection = (selection + 1) % 5;
}

void SettingsTimeMenu::button2() {
  if (selection == 4) {
    switchMenu(MENU_SETTINGS);
  } else {
    uint8_t hour = state.now.hour();
    uint8_t minute = state.now.minute();
    if (selection == 0) {
      hour = (hour + 1) % 24;
    } else if (selection == 1) {
      minute = (minute + 1) % 60;
    } else if (selection == 2) {
      hour = hour == 0 ? 23 : (hour - 1);
    } else if (selection == 3) {
      minute = minute == 0 ? 59 : (minute - 1);
    }
    state.now = DateTime(state.now.year(), state.now.month(), state.now.day(), hour, minute, 0);
    state.timeUpdated = true;
    state.timeMinuteUpdated = true;
    RTC.adjust(state.now);
  }
}

void SettingsTimeMenu::draw(Adafruit_GFX* display) const {
  char buff[9];
  // Border
  display->drawRect(0, 0, WIDTH, HEIGHT, WHITE);

  // Title
  display->setTextColor(WHITE);
  display->setTextSize(1);
  display->setCursor(2, 2);
  display_print(PSTR("Set Time"));
  display->drawFastVLine(50, 0, 10, WHITE);
  display->drawFastHLine(0, 10, 50, WHITE);

  // Time
  display->setTextSize(2);
  display->setCursor(20, 23);
  snprintf(buff, 9, "%02d:%02d", state.now.hour(), state.now.minute());
  display->print(buff);

  if (selection < 2) {
    display->fillTriangle(
      30 + (selection * 35), 15,
      35 + (selection * 35), 20,
      25 + (selection * 35), 20,
      WHITE);
  } else if (selection < 4) {
    display->fillTriangle(
      30 + ((selection - 2) * 35), 45,
      35 + ((selection - 2) * 35), 40,
      25 + ((selection - 2) * 35), 40,
      WHITE);
  }
  if (selection == 4) {
    display->fillRect(0, 53, WIDTH, 10, WHITE);
    display->setTextColor(BLACK, WHITE);
  } else {
    display->setTextColor(WHITE);
  }
  display->setTextSize(1);
  display->setCursor(5, 54);
  display_print(PSTR("Back"));
}

