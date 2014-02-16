
#define WHITE 1

#include <Arduino.h>
#include <Adafruit_GFX.h>
#include "Paddle.h"


Paddle::Paddle(int16_t x, int16_t y)
: w(2), h(14), x(x), y(y), d(0)
{}

void Paddle::update(int16_t target_y, int16_t ball_dx, bool should_miss) {
  bool my_direction = false;
  // Check side
  if (x == 0) {
    my_direction = ball_dx < 0;
  } else {
    my_direction = ball_dx > 0;
  }

  if (!my_direction) {
    d = 0;
  } else {
    // If I need to miss the ball, dont go straight to target
    if (should_miss) {
      if (target_y > 64 - h) {
        target_y = target_y - h;
      } else {
        target_y = target_y + h;
      }
    }
    if (y + h / 2 == target_y) {
      d = 0;
    } else if (y + h / 2 > target_y) {
      d = -1;
    } else {
      d = 1;
    }
  }

  if (y + h >= 64 && d == 1) {
    d = 0;
  } else if (y <= 0 && d == -1) {
    d = 0;
  }

  y = y + d;
}

void Paddle::draw(Adafruit_GFX& display) const {
  display.fillRect(x, y, w, h, WHITE);
}

