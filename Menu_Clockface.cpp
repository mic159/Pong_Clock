#include <Arduino.h>
#include <RTClib.h>
#include <Adafruit_GFX.h>
#include "Menu_Clockface.h"
#include "Menu.h"

extern RTC_DS1307 RTC;

ClockFaceMenu::ClockFaceMenu() {
  face = new PongGame();
  mode24h = false;
}
void ClockFaceMenu::onEnter() {
  DateTime now = RTC.now();
  uint8_t hour = now.hour();
  if (!mode24h && hour > 12) {
    hour = hour - 12;
  }
  face->setScore(hour, now.minute());
}
void ClockFaceMenu::update() {
  DateTime now = RTC.now();
  uint8_t hour = now.hour();
  if (!mode24h && hour > 12) {
    hour = hour - 12;
  }
  face->update(hour, now.minute());
}
void ClockFaceMenu::draw(Adafruit_GFX& display) const {
  face->draw(display);
}
void ClockFaceMenu::button1() {
  switchMenu(MENU_SETTINGS);
}
void ClockFaceMenu::button2() {
  switchMenu(MENU_SETTINGS);
}
