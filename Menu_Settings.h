#ifndef SETTINGS_MENU_H
#define SETTINGS_MENU_H

#include <Arduino.h>
#include <RTClib.h>
#include "Menu.h"

class SettingsMenu : public Menu {
public:
  SettingsMenu();
  virtual bool update();
  virtual void onEnter();
  virtual void button1();
  virtual void button2();
  virtual void draw(Adafruit_GFX* display) const;
protected:
  uint8_t selection;
  DateTime now;
  unsigned long last_check;
};

class Settings24hMenu : public Menu {
public:
  Settings24hMenu();
  virtual bool update();
  virtual void onEnter();
  virtual void button1();
  virtual void button2();
  virtual void draw(Adafruit_GFX* display) const;
private:
  DateTime now;
  unsigned long last_check;
};

class SettingsTimeMenu : public Menu {
public:
  SettingsTimeMenu();
  virtual bool update();
  virtual void onEnter();
  virtual void button1();
  virtual void button2();
  virtual void draw(Adafruit_GFX* display) const;
private:
  DateTime now;
  unsigned long last_check;
  uint8_t selection;
};

#endif
