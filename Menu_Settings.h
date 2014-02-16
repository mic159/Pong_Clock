#ifndef SETTINGS_MENU_H
#define SETTINGS_MENU_H

#include <Arduino.h>
#include "Menu.h"

class SettingsMenu : public Menu {
public:
  SettingsMenu();
  virtual void update();
  virtual void onEnter();
  virtual void button1();
  virtual void button2();
  virtual void draw(Adafruit_GFX& display) const;
protected:
  uint8_t selection;
  uint8_t _max;
};

class Settings24hMenu : public Menu {
public:
  Settings24hMenu();
  virtual void update();
  virtual void onEnter();
  virtual void button1();
  virtual void button2();
  virtual void draw(Adafruit_GFX& display) const;
};

class SettingsTimeMenu : public Menu {
public:
  SettingsTimeMenu();
  virtual void update();
  virtual void onEnter();
  virtual void button1();
  virtual void button2();
  virtual void draw(Adafruit_GFX& display) const;
};

#endif
