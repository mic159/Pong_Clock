#include <Arduino.h>
#include <RTClib.h>
#include <Adafruit_GFX.h>
#include "Menu_Clockface.h"
#include "Menu.h"

extern RTC_DS1307 RTC;

ClockFaceMenu::ClockFaceMenu()
: mode24h(false)
, last_check(0)
, now(0)
{
  face = new PongGame();
}
void ClockFaceMenu::onEnter() {
  now = RTC.now();
  last_check = millis();
  uint8_t hour = now.hour();
  if (!mode24h && hour > 12) {
    hour = hour - 12;
  }
  face->setScore(hour, now.minute());
}
bool ClockFaceMenu::update() {
  // Read RTC only once per second.
  if (millis() - last_check > 1000) {
    last_check = millis();
    now = RTC.now();
  }
  uint8_t hour = now.hour();
  if (!mode24h && hour > 12) {
    hour = hour - 12;
  }
  face->update(hour, now.minute());
  return true;
}
void ClockFaceMenu::draw(Adafruit_GFX* display) const {
  face->draw(display);
}
void ClockFaceMenu::button1() {
  switchMenu(MENU_SETTINGS);
}
void ClockFaceMenu::button2() {
  switchMenu(MENU_SETTINGS);
}

