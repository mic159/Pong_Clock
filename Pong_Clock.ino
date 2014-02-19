/*
 * Pongclock Code for Mike's custom sensor Board.
 * Original Code from https://github.com/rparrett/pongclock
 * Hardware adapted by KaR]V[aN, http://karman.cc
 * Modified by mic159
 *
 * Requirements:
 * Bounce2 https://github.com/thomasfredericks/Bounce-Arduino-Wiring
 * Adafruit GFX https://github.com/adafruit/Adafruit-GFX-Library
 * Adafriut SSD1306 https://github.com/adafruit/Adafruit_SSD1306
 * RTClib https://github.com/adafruit/RTClib
 */

#include <Bounce2.h>
#include <SPI.h>
#include <Wire.h>
#include <EEPROM.h>
#include <RTClib.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "Menu.h"
#include "Menu_Settings.h"
#include "Menu_Clockface.h"

#define OLED_RESET 4
#define MINUTE_PIN A1
#define HOUR_PIN   A2

Bounce btn1;
Bounce btn2;
Adafruit_SSD1306 display(OLED_RESET);
RTC_DS1307 RTC;

Menu* menus[MENU_MAX] = {};
Menu_selection current = MENU_CLOCK;

void switchMenu(Menu_selection s) {
  if (s >= MENU_MAX || menus[s] == NULL) return;
  current = s;
  menus[current]->onEnter();
}

Menu* getMenu(Menu_selection s) {
  return menus[s];
}

void setup(void) {
  Serial.begin(9600);
  randomSeed(analogRead(A3));
  Wire.begin();
  display.begin(SSD1306_SWITCHCAPVCC, 0x3c);
  display.clearDisplay();

  // First time init, set to code compile date.
  if (!RTC.isrunning()) {
    RTC.adjust(DateTime(__DATE__, __TIME__));
  }

  menus[MENU_CLOCK] = new ClockFaceMenu();
  menus[MENU_SETTINGS] = new SettingsMenu();
  //menus[MENU_SETTINGS_24H] = new Settings24hMenu();
  menus[MENU_SETTINGS_TIME] = new SettingsTimeMenu();

  // Setup buttons
  pinMode(MINUTE_PIN, INPUT);
  pinMode(HOUR_PIN, INPUT);
  btn1.attach(MINUTE_PIN);
  btn2.attach(HOUR_PIN);
  btn1.interval(30);
  btn2.interval(30);

  // Splash
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(5, 20);
  display.println("Pong Clock");
  display.setTextSize(1);
  display.setCursor(65, 56);
  display.println("by 0miker0");
  display.display();
  delay(2000);

  switchMenu(MENU_CLOCK);
}

void loop() {
  // As an optimisation, we only draw the display
  // when we really need to. Drawing the display
  // every time is wasteful if nothing has changed.
  bool draw = false;

  // Buttons
  if (btn1.update() && btn1.read()) {
    menus[current]->button1();
    draw = true;
  }
  if (btn2.update() && btn2.read()) {
    menus[current]->button2();
    draw = true;
  }

  // Update
  if(menus[current]->update()) {
    draw = true;
  }

  // Display
  if (draw) {
    display.clearDisplay();
    menus[current]->draw(&display);
    display.display();
  }
}

