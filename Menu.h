#ifndef MENU_H
#define MENU_H

#include <Arduino.h>
#include <Adafriut_GFX.h>

enum Menu_selection {
  MENU_CLOCK,
  MENU_SETTINGS,
  MENU_SETTINGS_24H,

  MENU_MAX
};

class Menu {
public:
  virtual void update() = 0;
  virtual void onEnter() = 0;
  virtual void draw(Adafruit_GFX& display) const = 0;
  virtual void button1() = 0;
  virtual void button2() = 0;
};

void switchMenu(Menu_selection);
Menu* getMenu(Menu_selection);

#endif
