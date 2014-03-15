/*
 * Tetris Clockface
 * Plays tetris on the side
 */

#include <Arduino.h>
#include <Adafruit_GFX.h>
#include "Clockface_Tetris.h"
#include "State.h"

#define WHITE 1
#define BLACK 0
#define WIDTH 128
#define HEIGHT 64

#define BOARD_HEIGHT 20
#define BOARD_WIDTH 10

#define BOARD_OFFSET_X 20

#define PEICE_MAX_HEIGHT 4
#define PEICE_MAX_WIDTH 2
#define PEICE_NUM 7

uint8_t PROGMEM gfx[PEICE_NUM * PEICE_MAX_HEIGHT] = {
    0b00000011
  , 0b00000011
  , 0b00000000
  , 0b00000000

  , 0b00000001
  , 0b00000001
  , 0b00000001
  , 0b00000001

  , 0b00000010
  , 0b00000011
  , 0b00000001
  , 0b00000000

  , 0b00000001
  , 0b00000011
  , 0b00000010
  , 0b00000000

  , 0b00000011
  , 0b00000001
  , 0b00000001
  , 0b00000000

  , 0b00000001
  , 0b00000001
  , 0b00000011
  , 0b00000000

  , 0b00000001
  , 0b00000011
  , 0b00000001
  , 0b00000000
};

ClockfaceTetris::ClockfaceTetris()
: peice(0)
, rotation(0)
, x(5), y(0)
, timer(0)
{
  memset(board, 0, sizeof(board));
}

void ClockfaceTetris::update(uint8_t hour, uint8_t minute) {
  // Decide move
  

  // Game logic
  if (checkCollision(0, 1)) {
    tileToBoard();
    y = 0;
    x = random(0, BOARD_WIDTH - PEICE_MAX_WIDTH + 1);
    peice = random(0, PEICE_NUM);
    if (checkCollision(0, 0)) {
      // Game Over
      memset(board, 0, sizeof(board));
    }
  } else {
    y += 1;
  }
}

bool ClockfaceTetris::checkCollision(int8_t xd, int8_t yd) const {
  // Out of bounds?
  if (y + yd >= BOARD_HEIGHT) return true;

  for (uint8_t iy=0; iy < PEICE_MAX_HEIGHT; ++iy) {
    for (uint8_t ix=0; ix < PEICE_MAX_WIDTH; ++ix) {
      if (pgm_read_byte(gfx + (peice * PEICE_MAX_HEIGHT) + iy) & _BV(ix)) {
        // Bounds
        if (y + yd + iy >= BOARD_HEIGHT) return true;
        if (y + yd + iy < 0) return true;
        if (x + xd + ix >= BOARD_WIDTH) return true;
        if (x + xd + ix < 0) return true;
        // Board collisions
        if (board[y + iy + yd] & _BV(x + xd + ix)) {
          return true;
        }
      }
    }
  }

  return false;
}

// Render the current peice onto the board bitmap
void ClockfaceTetris::tileToBoard() {
  for (uint8_t iy=0; iy < PEICE_MAX_HEIGHT; ++iy) {
    for (uint8_t ix=0; ix < PEICE_MAX_WIDTH; ++ix) {
      if (pgm_read_byte(gfx + (peice * PEICE_MAX_HEIGHT) + iy) & _BV(ix)) {
        board[y + iy] |= _BV(x + ix);
      }
    }
  }
}

void ClockfaceTetris::draw(Adafruit_GFX* display) const {
  char buff[8];

  // Game boarder
  display->drawFastVLine(BOARD_OFFSET_X - 2, 0, BOARD_HEIGHT * 3, WHITE);
  display->drawFastVLine(BOARD_OFFSET_X + BOARD_WIDTH * 3, 0, BOARD_HEIGHT * 3, WHITE);
  display->drawFastHLine(BOARD_OFFSET_X - 2, BOARD_HEIGHT * 3, BOARD_WIDTH * 3 + 3, WHITE);

  // Board bitmap
  for (uint8_t iy=0; iy < BOARD_HEIGHT; ++iy) {
    for (uint8_t ix=0; ix < BOARD_WIDTH; ++ix) {
      if (board[iy] & _BV(ix)) {
        display->fillRect(ix * 3 + BOARD_OFFSET_X, iy * 3, 2, 2, WHITE);
      }
    }
  }

  // Current peice
  for (uint8_t iy=0; iy < PEICE_MAX_HEIGHT; ++iy) {
    for (uint8_t ix=0; ix < PEICE_MAX_WIDTH; ++ix) {
      if (pgm_read_byte(gfx + (peice * PEICE_MAX_HEIGHT) + iy) & _BV(ix)) {
        display->fillRect((ix + x) * 3 + BOARD_OFFSET_X, (iy + y) * 3, 2, 2, WHITE);
      }
    }
  }

  // Time
  display->setTextColor(WHITE);
  display->setTextSize(1);
  display->setCursor(70, 2);
  display->print(F("Time:"));
  display->setCursor(80, 10);
  if (state.mode24h) {
    snprintf_P(buff, 7, PSTR("%02d:%02d"), state.now.hour(), state.now.minute());
  } else {
    uint8_t hour = state.now.hour() % 12;
    if (hour == 0) hour = 12;
    snprintf_P(buff, 7, PSTR("%02d:%02d%c"),
      hour,
      state.now.minute(),
      state.now.hour() > 11 ? 'p' : 'a'
      );
  }
  display->print(buff);

  // Date
  display->setCursor(70, 20);
  display->print(F("Date:"));
  display->setCursor(80, 28);
  display->print(state.now.day());
  display->print(' ');
  display->print(state.getMonthStr(state.now.month()));
  //display->print(state.getDayStr(state.now.dayOfWeek()));
}

