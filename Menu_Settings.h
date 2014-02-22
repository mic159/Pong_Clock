#ifndef MENU_SETTINGS_H
#define MENU_SETTINGS_H

#include <Arduino.h>
#include "Menu.h"

class SettingsMenu : public Menu {
public:
  SettingsMenu();
  virtual bool update();
  virtual void button1();
  virtual void button2();
  virtual void draw(Adafruit_GFX* display) const;
protected:
  uint8_t selection;
};

#endif
