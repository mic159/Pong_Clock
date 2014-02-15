/*
 * Pongclock Code for Mike's custom sensor Board.
 * Original Code from https://github.com/rparrett/pongclock
 * Hardware adapted by KaR]V[aN, http://karman.cc
 *
 */

#include <Bounce.h>    // http://playground.arduino.cc/code/bounce
#include <SPI.h>
#include <Wire.h>
#include <EEPROM.h>
#include <RTClib.h> // https://github.com/adafruit/RTClib
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "Paddle.h"
#include "Ball.h"
#include "PongGame.h"

#define OLED_RESET 4

const int BUTTON_MINUTE = A1;  //11
const int BUTTON_HOUR = A2;     //5

Bounce minutebouncer = Bounce(BUTTON_MINUTE, 20);
Bounce hourbouncer = Bounce(BUTTON_HOUR, 20);
Adafruit_SSD1306 display(OLED_RESET);
RTC_DS1307 RTC;

const int16_t h = 64;
const int16_t w = 128;

int16_t hour = 12;
int16_t minute = 4;
volatile int16_t second = 0;

PongGame game;


void setup(void) 
{
  randomSeed(millis()*analogRead(0));
  for (int i=0;i<255;i++)
    random(0,random(255));
   
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
  
  pinMode(BUTTON_MINUTE, INPUT);
  digitalWrite(BUTTON_MINUTE, HIGH);
  pinMode(BUTTON_HOUR, INPUT);
  digitalWrite(BUTTON_HOUR, HIGH);
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
  display.clearDisplay();   // clears the screen and buffer
}

void buttons() {
  minutebouncer.update();
  if (minutebouncer.risingEdge()) {
    minute = minute + 1;
    if (minute > 59) minute = 0;
    
    second = 0;
    
    game.setScore(hour, minute);
    
    setclock();
  }
  
  hourbouncer.update();
  if (hourbouncer.risingEdge()) {
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
