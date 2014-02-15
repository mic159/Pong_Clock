/*
 * Pongclock Code for Mike's custom sensor Board.
 * Original Code from https://github.com/rparrett/pongclock
 * Hardware adapted by KaR]V[aN, http://karman.cc
 * Modified by mic159
 *
 * Requirements:
 * Bounce http://playground.arduino.cc/code/bounce
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

#include "Paddle.h"
#include "Ball.h"
#include "PongGame.h"

#define OLED_RESET 4

const int MINUTE_PIN = A1;
const int HOUR_PIN = A2;

Bounce minuteBtn;
Bounce hourBtn;
Adafruit_SSD1306 display(OLED_RESET);
RTC_DS1307 RTC;

int16_t hour;
int16_t minute;
int16_t second;

PongGame game;


void setup(void) {
  randomSeed(analogRead(A3));
  Wire.begin();
  display.begin(SSD1306_SWITCHCAPVCC, 0x3c);
  display.setTextSize(2);
  display.clearDisplay();
  if (!RTC.isrunning()) {
    RTC.adjust(DateTime(__DATE__, __TIME__));
  }
  RTC.begin();
  readclock();

  game.setScore(hour, minute);
  
  // Setup buttons
  pinMode(MINUTE_PIN, INPUT_PULLUP);
  pinMode(HOUR_PIN, INPUT_PULLUP);
  minuteBtn.attach(MINUTE_PIN);
  hourBtn.attach(HOUR_PIN);
  minuteBtn.interval(30);
  hourBtn.interval(30);
}

void readclock() {
  DateTime now = RTC.now();

  hour   = now.hour();
  minute = now.minute();
  second = now.second();

  if (hour > 12) hour = hour - 12;
}

void loop() {
  readclock();
  game.update(hour, minute);
  game.draw(display);

  buttons();

  display.display();
  display.clearDisplay();
}

void buttons() {
  if (minuteBtn.update() && minuteBtn.read()) {
    minute = (minute + 1) % 60;
    second = 0;
    game.setScore(hour, minute);
    setclock();
  }

  if (minuteBtn.update() && minuteBtn.read()) {
    hour = hour + 1;
    if (hour > 12) hour = 1;
    game.setScore(hour, minute);
    setclock();
  }
}

void setclock() {
  DateTime now = RTC.now();
  DateTime updated = DateTime(now.year(), now.month(), now.day(), hour, minute, now.second());
  RTC.adjust(updated);
  RTC.begin();
}

