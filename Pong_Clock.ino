/*
 * Pongclock Code for 0miker0's Pong Clock.
 * Code by mic159.
 *
 * Requirements:
 * Bounce2 (v1.2.0) https://github.com/thomasfredericks/Bounce2
 * Adafruit GFX (v1.1.5) https://github.com/adafruit/Adafruit-GFX-Library
 * Adafriut SSD1306 (v1.1.0) https://github.com/adafruit/Adafruit_SSD1306
 * RTClib (v1.2.0) https://github.com/adafruit/RTClib
 * You can install them via the Arduino library manager.
 * 
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
#include "Menu_Settings_24.h"
#include "Menu_Settings_Time.h"
#include "Menu_Settings_Date.h"
#include "Menu_Settings_Brightness.h"
#include "Menu_Clockface.h"
#include "State.h"

#define OLED_RESET 4
#define MINUTE_PIN A1
#define HOUR_PIN   A2
#define WIDTH 128
#define HEIGHT 64

// Set this to enable printing debug stats to the screen
//#define DEBUG_STATS

Bounce btn1;
Bounce btn2;
Adafruit_SSD1306 display(OLED_RESET);
RTC_DS1307 RTC;

State state;
Menu* menu = NULL;

void setup(void) {
  Serial.begin(9600);
  randomSeed(analogRead(A3));
  Wire.begin();
  display.begin(SSD1306_SWITCHCAPVCC, 0x3c);
  display.clearDisplay();
  display.setRotation(2);

  // First time init, set to code compile date.
  if (!RTC.isrunning()) {
    RTC.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

  // Setup buttons
  pinMode(MINUTE_PIN, INPUT);
  pinMode(HOUR_PIN, INPUT);
  btn1.attach(HOUR_PIN);
  btn2.attach(MINUTE_PIN);
  btn1.interval(30);
  btn2.interval(30);

  // Splash
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(5, 20);
  display.print(F("Pong Clock"));
  display.setTextSize(1);
  display.setCursor(66, 48);
  display.print(F("by 0miker0"));
  display.setCursor(78, 56);
  display.print(F("& mic159"));
  display.display();
  delay(2000);

  // Load things from state
  display.dim(state.dim);

  state.update();
  switchMenu(MENU_CLOCK);
}

void loop() {
  // As an optimisation, we only draw the display
  // when we really need to. Drawing the display
  // every time is wasteful if nothing has changed.
  bool draw = false;
#ifdef DEBUG_STATS
  unsigned long timer = millis();
#endif

  // Buttons
  if (btn1.update() && btn1.read()) {
    menu->button1();
    draw = true;
  }
  if (btn2.update() && btn2.read()) {
    menu->button2();
    draw = true;
  }

  // Switch menu if indicated.
  updateMenuSelection();

  // Update
  state.update();
  if(menu->update()) {
    draw = true;
  }

  // Display
  if (draw) {
    display.clearDisplay();
    menu->draw(&display);

#ifdef DEBUG_STATS
    display.setTextSize(1);
    display.setTextColor(WHITE, BLACK);
    display.setCursor(80, HEIGHT - 10);
    display.print(freeRam());
    display.setCursor(109, HEIGHT - 10);
    display.print(millis() - timer);
#endif

    display.display();
  }

}

#ifdef DEBUG_STATS
int freeRam () {
  extern int __heap_start, *__brkval;
  int v;
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
}
#endif

