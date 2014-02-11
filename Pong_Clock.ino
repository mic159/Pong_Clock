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
#include "RTClib.h" // https://github.com/adafruit/RTClib
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET 4

const int minutebtn = A1;  //11
const int hourbtn = A2;     //5

Bounce minutebouncer = Bounce(minutebtn, 20);
Bounce hourbouncer = Bounce(hourbtn, 20);
Adafruit_SSD1306 display(OLED_RESET);
RTC_DS1307 RTC;

const int16_t h = 64;
const int16_t w = 128;

int pause = 0;

int16_t paddle_h = 14;
int16_t paddle_w = 2;

int16_t lpaddle_x = 0;
int16_t rpaddle_x = w - paddle_w;

int16_t lpaddle_y = 0;
int16_t rpaddle_y = h - paddle_h;

int16_t lpaddle_d = 1;
int16_t rpaddle_d = -1;

int16_t lpaddle_ball_t = w - w / 4;
int16_t rpaddle_ball_t = w / 4;

int16_t target_y = 0;

int16_t ball_x = 2;
int16_t ball_y = 2;
int16_t ball_dx = 1;
int16_t ball_dy = 1;
int16_t ball_w = 3;
int16_t ball_h = 3;

int16_t dashline_h = 4;
int16_t dashline_w = 2;
int16_t dashline_n = h / dashline_h;
int16_t dashline_x = w / 2 - 1;
int16_t dashline_y = dashline_h / 2;

int16_t lscore = 12;
int16_t rscore = 4;

byte newminute = 0;
byte newhour = 0;

int16_t hour = 12;
int16_t minute = 4;
volatile int16_t second = 0;

int16_t font_s  = 2;
int16_t font_w  = 5 * font_s;
int16_t font_h  = 7 * font_s;
int16_t font_sp = 2 * font_s;


void setup(void) 
{
  randomSeed(millis()*analogRead(0));
  for (int i=0;i<255;i++)
    random(0,random(255));
   
  Wire.begin();
  //TWBR = 2;  //SPEED?

  // initialize with the I2C addr 0x3D (for the 128x64)
  display.begin(SSD1306_SWITCHCAPVCC, 0x3c);

  initclock();
  initgame();

  display.setTextSize(2);
  display.clearDisplay();   // clears the screen and buffer  
  
  pinMode(minutebtn, INPUT);
  digitalWrite(minutebtn, HIGH);
  pinMode(hourbtn, INPUT);
  digitalWrite(hourbtn, HIGH);
}

void initclock() {
  if (! RTC.isrunning()) {
    RTC.adjust(DateTime(__DATE__, __TIME__));
  }
  
  RTC.begin();
  
  DateTime now = RTC.now();

  hour   = now.hour();
  minute = now.minute();
  second = now.second();
 
  if (hour > 12) hour = hour - 12;
  
}

void initgame() {
  lpaddle_y = random(0, h - paddle_h);
  rpaddle_y = random(0, h - paddle_h);

  // ball is placed on the center of the left paddle
  ball_y = lpaddle_y + (paddle_h / 2);
  
  lscore = hour;
  rscore = minute;

  calc_target_y();
}

void loop() {
  keeptime();
  initclock();
  lpaddle();
  rpaddle();
  ball();
  midline();
  score();

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
    
    newminute = 1;
    
    changescore(false);
    
    setclock();
  }
  
  hourbouncer.update();
  if (hourbouncer.risingEdge()) {
    hour = hour + 1;
    if (hour > 12) hour = 1;
    
    newhour = 1;
    
    changescore(false);
    
    setclock();
  }
  
}

void setclock() {
  DateTime now = RTC.now();
  DateTime updated = DateTime(now.year(), now.month(), now.day(), hour, minute, now.second());
  RTC.adjust(updated);
  RTC.begin();
}

void keeptime() {
  if (second >= 59) {
    newminute = 1;

    second = 0;
    
    minute = minute + 1;
  }

  if (minute == 00) {
    newminute = 0;
    newhour = 1;
    
    minute = 0;
    hour = hour + 1;
  }

  if (hour > 12) {
    hour = 1;
  }
}

void midline() {
  for(int16_t i = 0; i < dashline_n; i = i + 2) {
    display.fillRect(dashline_x, dashline_y + i * dashline_h, dashline_w, dashline_h, WHITE);
  }
}

void score(void) {
  int lscore_x, rscore_x;

  if (lscore > 9) lscore_x = w / 2 - font_w - font_w - font_sp - 10;
  else lscore_x = w / 2 - font_w - 10;

  rscore_x = w / 2 + 10;

  display.setTextColor(WHITE);

  display.setCursor(lscore_x, dashline_y);
  display.println(lscore);

  display.setCursor(rscore_x, dashline_y);
  display.println(rscore);
}

void lpaddle() {
  if (pause > 0) {
        display.fillRect(lpaddle_x, lpaddle_y, paddle_w, paddle_h, WHITE);
        return;
  }

  lpaddle_y = lpaddle_y + lpaddle_d;

  if (ball_dx == 1) lpaddle_d = 0;
  else {
    if (lpaddle_y + paddle_h / 2 == target_y) lpaddle_d = 0;
    else if (lpaddle_y + paddle_h / 2 > target_y) lpaddle_d = -1;
    else lpaddle_d = 1;
  }

  if (lpaddle_y + paddle_h >= h && lpaddle_d == 1) lpaddle_d = 0;
  else if (lpaddle_y <= 0 && lpaddle_d == -1) lpaddle_d = 0;

  display.fillRect(lpaddle_x, lpaddle_y, paddle_w, paddle_h, WHITE);
}

void rpaddle() {
  if (pause > 0) {
    display.fillRect(rpaddle_x, rpaddle_y, paddle_w, paddle_h, WHITE);
    return;
  }

  rpaddle_y = rpaddle_y + rpaddle_d;

  if (ball_dx == -1) rpaddle_d = 0;
  else {
    if (rpaddle_y + paddle_h / 2 == target_y) rpaddle_d = 0;
    else if (rpaddle_y + paddle_h / 2 > target_y) rpaddle_d = -1;
    else rpaddle_d = 1;
  }

  if (rpaddle_y + paddle_h >= h && rpaddle_d == 1) rpaddle_d = 0;
  else if (rpaddle_y <= 0 && rpaddle_d == -1) rpaddle_d = 0;

  display.fillRect(rpaddle_x, rpaddle_y, paddle_w, paddle_h, WHITE);
}

void calc_target_y() {
  int16_t target_x;
  int16_t reflections;
  int16_t y;

  if (ball_dx == 1) {
    target_x = w - ball_w;
  } 
  else {
    target_x = -1 * (w - ball_w);
  }

  y = abs(target_x * (ball_dy / ball_dx) + ball_y);

  reflections = floor(y / h);

  if (reflections % 2 == 0) {
    target_y = y % h;
  } 
  else {
    target_y = h - (y % h);
  }
  
  // when the time changes, we want to dodge the ball
  // instead of seeking it.
  
  if (newminute && ball_dx == -1) {
    if (target_y > h - paddle_h) {
      target_y = target_y - paddle_h;
    } else {
      target_y = target_y + paddle_h;
    }
  } else if (newhour && ball_dx == 1) {
    if (target_y > h - paddle_h) {
      target_y = target_y - paddle_h;
    } else {
      target_y = target_y + paddle_h;
    }
  }
}

void changescore(int newgame) {
  score();

  if (newhour) {
    lscore = hour;
    rscore = minute;
    newhour = 0;
    newminute = 0;
  } else if (newminute) {
    rscore = minute;
    newminute = 0;
  }
  
  if (newgame) {
    lpaddle_y = random(0, h - paddle_h);

    pause = 50;
    
    ball_x = paddle_w;
    ball_y = lpaddle_y + (paddle_h / 2);
    ball_dx = 1;

    calc_target_y();
  }
}

void ball() {
  if (pause > 0) {
    pause = pause - 1;
    return; 
  } 

  ball_x = ball_x + ball_dx;
  ball_y = ball_y + ball_dy;

  if (ball_dx == -1 && ball_x == paddle_w && ball_y + ball_h >= lpaddle_y && ball_y <= lpaddle_y + paddle_h) {
    ball_dx = ball_dx * -1;
    
    calc_target_y(); 
  } else if (ball_dx == 1 && ball_x + ball_w == w - paddle_w && ball_y + ball_h >= rpaddle_y && ball_y <= rpaddle_y + paddle_h) {
    ball_dx = ball_dx * -1;
    
    calc_target_y();
  } else if ((ball_dx == 1 && ball_x >= w) || (ball_dx == -1 && ball_x + ball_w < 0)) {
    changescore(1);
  }

  if (ball_y > h - ball_w || ball_y < 0) {
    ball_dy = ball_dy * -1;
  }

  display.fillRect(ball_x, ball_y, ball_w, ball_h, WHITE);
}
