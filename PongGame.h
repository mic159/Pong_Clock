#ifndef PONG_GAME_H
#define PONG_GAME_H

#include <Adafruit_GFX.h>
#include "Paddle.h"
#include "Ball.h"

class PongGame {
public:
  PongGame();

  void setScore(uint8_t hour, uint8_t minute);
  void update(uint8_t hour, uint8_t minute);
  void draw(Adafruit_GFX* display) const;
  void drawScore(Adafruit_GFX* display) const;
private:
  Paddle lpaddle, rpaddle;
  Ball ball;
  uint8_t lscore, rscore;
  uint8_t pause;
};

#endif
