#ifndef MENU_CLOCKFACE_H
#define MENU_CLOCKFACE_H

#include <Arduino.h>
#include "Menu.h"
#include "PongGame.h"

class ClockFaceMenu : public Menu {
public:
  ClockFaceMenu();
  virtual void onEnter();
  virtual void update();
  virtual void draw(Adafruit_GFX& display) const;
  virtual void button1();
  virtual void button2();

  PongGame* face;
  bool mode24h;
};

#endif
