#ifndef PONG_GAME_H
#define PONG_GAME_H

#include <Adafruit_GFX.h>
#include "Clockface.h"

class ClockfaceDigital : public Clockface {
public:
  void draw(Adafruit_GFX* display) const;
};

#endif
