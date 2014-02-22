#ifndef MENU_CLOCKFACE_H
#define MENU_CLOCKFACE_H

#include <Arduino.h>
#include <RTClib.h>
#include "Menu.h"
#include "Clockface.h"

class ClockFaceMenu : public Menu {
public:
  ClockFaceMenu();
  ~ClockFaceMenu();

  bool update();
  void draw(Adafruit_GFX* display) const;
  void button1();
  void button2();

  Clockface* face;
};

#endif
