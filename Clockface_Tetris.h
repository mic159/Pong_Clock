#ifndef CLOCKFACE_TETRIS_H
#define CLOCKFACE_TETRIS_H

#include <Adafruit_GFX.h>
#include "Clockface.h"

class ClockfaceTetris : public Clockface {
public:
  ClockfaceTetris();
  void update(uint8_t hour, uint8_t minute);
  void draw(Adafruit_GFX* display) const;
private:
  bool checkCollision(int8_t xd, int8_t yd) const;
  void tileToBoard();
  uint16_t board[20];
  uint8_t peice, rotation;
  int8_t x, y;
  uint8_t timer;
};

#endif
