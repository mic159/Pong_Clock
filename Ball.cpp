
#define WHITE 1

#include <Arduino.h>
#include <Adafruit_GFX.h>
#include "Paddle.h"
#include "Ball.h"

Ball::Ball(int16_t x, int16_t y, int16_t dx, int16_t dy)
: x(x), y(y), w(3), h(3), dx(dx), dy(dy)
{}

bool Ball::update(const Paddle& lpaddle, const Paddle& rpaddle) {
  x = x + dx;
  y = y + dy;
  bool hit = false;

  if (dx == -1 && x == lpaddle.w && y + h >= lpaddle.y && y <= lpaddle.y + lpaddle.h) {
    dx = dx * -1;
    calcTarget();
  } else if (dx == 1 && x + w == 128 - rpaddle.w && y + h >= rpaddle.y && y <= rpaddle.y + rpaddle.h) {
    dx = dx * -1;
    calcTarget();
  } else if ((dx == 1 && x >= 128) || (dx == -1 && x + w < 0)) {
    hit = true;
  }

  if (y > 64 - w || y < 0) {
    dy = dy * -1;
  }
  return hit;
}

void Ball::calcTarget() {
  int16_t target_x;
  int16_t reflections;
  int16_t calc_y;

  if (dx == 1) {
    target_x = 128 - w;
  } 
  else {
    target_x = -1 * (128 - w);
  }

  calc_y = abs(target_x * (dy / dx) + y);

  reflections = floor(calc_y / 64);

  if (reflections % 2 == 0) {
    target_y = calc_y % 64;
  } 
  else {
    target_y = 64 - (calc_y % 64);
  }
}

void Ball::draw(Adafruit_GFX* display) const {
  display->fillRect(x, y, w, h, WHITE);
}

