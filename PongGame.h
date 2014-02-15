#ifndef PONG_GAME_H
#define PONG_GAME_H

#include <arduino.h>
#include <Adafruit_GFX.h>
#include "Paddle.h"
#include "Ball.h"

class PongGame {
public:
  PongGame();

  void setScore(uint16_t hour, uint16_t minute);

  void update(uint16_t hour, uint16_t minute);

  void draw(Adafruit_GFX& display) const;
  
  void drawScore(Adafruit_GFX& display) const;
private:
  Paddle lpaddle, rpaddle;
  Ball ball;
  int16_t lscore, rscore;
  uint8_t pause;
};

#endif
