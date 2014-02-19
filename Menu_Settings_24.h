#ifndef MENU_SETTINGS_24_H
#define MENU_SETTINGS_24_H

#include <Arduino.h>
#include "Menu.h"

class Settings24hMenu : public Menu {
public:
  Settings24hMenu();
  virtual bool update();
  virtual void onEnter();
  virtual void button1();
  virtual void button2();
  virtual void draw(Adafruit_GFX* display) const;
};

#endif
