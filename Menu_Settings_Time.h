#ifndef MENU_SETTINGS_TIME_H
#define MENU_SETTINGS_TIME_H

#include <Arduino.h>
#include "Menu.h"

class SettingsTimeMenu : public Menu {
public:
  SettingsTimeMenu();
  virtual bool update();
  virtual void onEnter();
  virtual void button1();
  virtual void button2();
  virtual void draw(Adafruit_GFX* display) const;
private:
  uint8_t selection;
};

#endif
