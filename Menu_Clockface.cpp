#include <Arduino.h>
#include <RTClib.h>
#include <Adafruit_GFX.h>
#include "Menu_Clockface.h"
#include "Menu.h"
#include "State.h"

ClockFaceMenu::ClockFaceMenu() {
  face = new PongGame();
  uint8_t hour = state.now.hour();
  if (!state.mode24h && hour > 12) {
    hour = hour - 12;
  }
  face->setScore(hour, state.now.minute());
}

ClockFaceMenu::~ClockFaceMenu() {
  delete face;
}

bool ClockFaceMenu::update() {
  uint8_t hour = state.now.hour();
  if (!state.mode24h && hour > 12) {
    hour = hour - 12;
  }
  face->update(hour, state.now.minute());
  // Always render
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

