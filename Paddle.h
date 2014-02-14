#ifndef PADDLE_H
#define PADDLE_H

#include <arduino.h>
#include <Adafruit_GFX.h>

struct Paddle {
  Paddle(int16_t x, int16_t y)
  : w(2), h(14), x(x), y(y), d(0)
  {}

  void update(int16_t target_y, int16_t ball_dx, bool should_miss) {
    y = y + d;
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

    if (y + h >= h && d == 1) {
      d = 0;
    } else if (y <= 0 && d == -1) {
      d = 0;
    }
  }

  void draw(Adafruit_GFX& display) const {
    display.fillRect(x, y, w, h, WHITE);
  }

  int16_t x, y, w, h;
  int16_t d;
};

#endif
