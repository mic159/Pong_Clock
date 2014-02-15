#ifndef PADDLE_H
#define PADDLE_H

#include <Adafruit_GFX.h>

struct Paddle {
  Paddle(int16_t x, int16_t y);
  void update(int16_t target_y, int16_t ball_dx, bool should_miss);
  void draw(Adafruit_GFX& display) const;

  int16_t x, y, w, h;
  int16_t d;
};

#endif
