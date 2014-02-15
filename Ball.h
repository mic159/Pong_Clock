#ifndef BALL_H
#define BALL_H

#include <arduino.h>
#include <Adafruit_GFX.h>
#include "Paddle.h"

struct Ball {
  Ball(int16_t x, int16_t y, int16_t dx, int16_t dy);
  
  bool update(const Paddle& lpaddle, const Paddle& rpaddle);
  void calcTarget();
  void draw(Adafruit_GFX& display) const;

  int16_t x, y, w, h;
  int16_t dx, dy;
  int16_t target_y;
};

#endif
