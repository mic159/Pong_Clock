#include <Arduino.h>
#include <Adafruit_GFX.h>
#include "Menu_Settings_Date.h"
#include "State.h"

// Grab RTC instance from .ino
extern RTC_DS1307 RTC;

// Some graphics constants
#define BLACK 0
#define WHITE 1
#define WIDTH 128
#define HEIGHT 64

const __FlashStringHelper* getMonthStr(uint8_t m) {
  switch (m) {
    default:
    case 0: return F("Jan");
    case 1: return F("Feb");
    case 2: return F("Mar");
    case 3: return F("Apr");
    case 4: return F("May");
    case 5: return F("Jun");
    case 6: return F("Jul");
    case 7: return F("Aug");
    case 8: return F("Sep");
    case 9: return F("Oct");
    case 10: return F("Nov");
    case 11: return F("Dec");
  }
}

SettingsDateMenu::SettingsDateMenu()
: selection(0)
{}

bool SettingsDateMenu::update() {
  return state.timeMinuteUpdated;
}

void SettingsDateMenu::onEnter() {
  selection = 0;
}

void SettingsDateMenu::button1() {
  selection = (selection + 1) % 7;
}

void SettingsDateMenu::button2() {
  if (selection == 6) {
    switchMenu(MENU_SETTINGS);
  } else {
    uint8_t year = state.now.year();
    uint8_t month = state.now.month();
    uint8_t day = state.now.day();
    if (selection == 0) {
      year += 1;
    } else if (selection == 3) {
      year -= 1;
    } else if (selection == 1) {
      month = (month + 1) % 12;
    } else if (selection == 4) {
      month = month == 0 ? 11 : (month - 1);
    } else if (selection == 2) {
      day = (day + 1) % 31;
    } else if (selection == 5) {
      day = day == 0 ? 31 : (day - 1);
    }
    state.now = DateTime(year, month, day, state.now.hour(), state.now.minute(), state.now.second());
    state.timeUpdated = true;
    state.timeMinuteUpdated = true;
    RTC.adjust(state.now);
  }
}

void SettingsDateMenu::draw(Adafruit_GFX* display) const {
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

  // Time
  display->setTextSize(2);
  display->setCursor(0, 23);
  display->print(state.now.year());
  display->print(F(" "));
  display->print(getMonthStr(state.now.month()));
  display->print(F(" "));
  display->print(state.now.day());

  if (selection < 3) {
    display->fillTriangle(
      30 + (selection * 35), 15,
      35 + (selection * 35), 20,
      25 + (selection * 35), 20,
      WHITE);
  } else if (selection < 6) {
    display->fillTriangle(
      30 + ((selection - 3) * 35), 45,
      35 + ((selection - 3) * 35), 40,
      25 + ((selection - 3) * 35), 40,
      WHITE);
  }
  if (selection == 6) {
    display->fillRect(0, 53, WIDTH, 10, WHITE);
    display->setTextColor(BLACK, WHITE);
  } else {
    display->setTextColor(WHITE);
  }
  display->setTextSize(1);
  display->setCursor(5, 54);
  display->print(F("Back"));
}

