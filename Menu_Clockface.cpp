#include <Arduino.h>
#include <RTClib.h>
#include <Adafruit_GFX.h>
#include "Menu_Clockface.h"
#include "Menu.h"
#include "State.h"
#include "Clockface_Pong.h"
#include "Clockface_Digital.h"

enum FACE {
  FACE_PONG,
  FACE_DIGITAL,

  FACE_MAX
};

ClockfaceMenu::ClockfaceMenu()
: faceType(FACE_PONG)
{
  face = new ClockfacePong();
  uint8_t hour = state.now.hour();
  if (!state.mode24h && hour > 12) {
    hour = hour - 12;
  }
  face->begin(hour, state.now.minute());
}

ClockfaceMenu::~ClockfaceMenu() {
  delete face;
}

bool ClockfaceMenu::update() {
  uint8_t hour = state.now.hour();
  if (!state.mode24h && hour > 12) {
    hour = hour - 12;
  }
  face->update(hour, state.now.minute());
  // Always render
  return true;
}

void ClockfaceMenu::draw(Adafruit_GFX* display) const {
  face->draw(display);
}

void ClockfaceMenu::button1() {
  faceType = (faceType + 1) % FACE_MAX;

  // Switch object
  delete face;
  switch(faceType) {
  case FACE_PONG:
    face = new ClockfacePong();
    break;
  case FACE_DIGITAL:
    face = new ClockfaceDigital();
    break;
  }

  // Call begin
  uint8_t hour = state.now.hour();
  if (!state.mode24h && hour > 12) {
    hour = hour - 12;
  }
  face->begin(hour, state.now.minute());
}

void ClockfaceMenu::button2() {
  switchMenu(MENU_SETTINGS);
}

