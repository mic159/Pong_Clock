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
#define PEICE_MAX_WIDTH 4
#define PEICE_NUM 7

const uint8_t PROGMEM gfx[PEICE_NUM * PEICE_MAX_HEIGHT * 4] = {
  // Square
    0b00000011
  , 0b00000011
  , 0b00000000
  , 0b00000000
  
  , 0b00000011
  , 0b00000011
  , 0b00000000
  , 0b00000000
  
  , 0b00000011
  , 0b00000011
  , 0b00000000
  , 0b00000000
  
  , 0b00000011
  , 0b00000011
  , 0b00000000
  , 0b00000000

  // Long
  , 0b00000001
  , 0b00000001
  , 0b00000001
  , 0b00000001

  , 0b00001111
  , 0b00000000
  , 0b00000000
  , 0b00000000

  , 0b00000001
  , 0b00000001
  , 0b00000001
  , 0b00000001

  , 0b00001111
  , 0b00000000
  , 0b00000000
  , 0b00000000

  // S
  , 0b00000010
  , 0b00000011
  , 0b00000001
  , 0b00000000

  , 0b00000011
  , 0b00000110
  , 0b00000000
  , 0b00000000

  , 0b00000010
  , 0b00000011
  , 0b00000001
  , 0b00000000

  , 0b00000011
  , 0b00000110
  , 0b00000000
  , 0b00000000

  // Z
  , 0b00000001
  , 0b00000011
  , 0b00000010
  , 0b00000000

  , 0b00000110
  , 0b00000011
  , 0b00000000
  , 0b00000000

  , 0b00000001
  , 0b00000011
  , 0b00000010
  , 0b00000000

  , 0b00000110
  , 0b00000011
  , 0b00000000
  , 0b00000000

  // L
  , 0b00000011
  , 0b00000001
  , 0b00000001
  , 0b00000000

  , 0b00000001
  , 0b00000111
  , 0b00000000
  , 0b00000000

  , 0b00000010
  , 0b00000010
  , 0b00000011
  , 0b00000000

  , 0b00000111
  , 0b00000100
  , 0b00000000
  , 0b00000000

  // 1
  , 0b00000001
  , 0b00000001
  , 0b00000011
  , 0b00000000

  , 0b00000100
  , 0b00000111
  , 0b00000000
  , 0b00000000

  , 0b00000011
  , 0b00000010
  , 0b00000010
  , 0b00000000

  , 0b00000111
  , 0b00000001
  , 0b00000000
  , 0b00000000

  // T
  , 0b00000001
  , 0b00000011
  , 0b00000001
  , 0b00000000

  , 0b00000010
  , 0b00000111
  , 0b00000000
  , 0b00000000

  , 0b00000010
  , 0b00000011
  , 0b00000010
  , 0b00000000

  , 0b00000111
  , 0b00000010
  , 0b00000000
  , 0b00000000
};

ClockfaceTetris::ClockfaceTetris()
: peice(0)
, rotation(0)
, x(5), y(0)
, score(0)
, highscore(0)
, targetX(5)
, targetRotation(0)
{
  memset(board, 0, sizeof(board));
  decideMove();
}

void ClockfaceTetris::update(uint8_t hour, uint8_t minute) {
  // AI
  if (x < targetX && !checkCollision(1, 0, rotation)) {
    x += 1;
  } else if (x > targetX && !checkCollision(-1, 0, rotation)) {
    x -= 1;
  }
  
  if (rotation != targetRotation && !checkCollision(0, 0, (rotation + 1) % 4)) {
    rotation = (rotation + 1) % 4;
  }

  // Game logic
  if (checkCollision(0, 1, rotation)) {
    tileToBoard();
    score += 5;
    clearLines();
    y = 0;
    x = 5;
    peice = random(0, PEICE_NUM);
    if (checkCollision(0, 0, rotation)) {
      // Game Over
      if (score > highscore) {
        highscore = score;
      }
      memset(board, 0, sizeof(board));
      score = 0;
    }
    decideMove();
  } else {
    y += 1;
  }
}

bool ClockfaceTetris::checkCollision(int8_t xd, int8_t yd, uint8_t r) const {
  // Out of bounds?
  if (y + yd >= BOARD_HEIGHT) return true;
  if (x + xd < 0) return true;
  if (x + xd >= BOARD_WIDTH) return true;

  for (uint8_t iy=0; iy < PEICE_MAX_HEIGHT; ++iy) {
    uint8_t row = pgm_read_byte(gfx + (peice * 4 + r) * PEICE_MAX_HEIGHT + iy);
    for (uint8_t ix=0; ix < PEICE_MAX_WIDTH; ++ix) {
      if (row & _BV(ix)) {
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

  // Didnt find any collisions.
  return false;
}

// Render the current peice onto the board bitmap
void ClockfaceTetris::tileToBoard() {
  for (uint8_t iy=0; iy < PEICE_MAX_HEIGHT; ++iy) {
    uint8_t row = pgm_read_byte(gfx + (peice * 4 + rotation) * PEICE_MAX_HEIGHT + iy);
    for (uint8_t ix=0; ix < PEICE_MAX_WIDTH; ++ix) {
      if (row & _BV(ix)) {
        board[y + iy] |= _BV(x + ix);
      }
    }
  }
}

void ClockfaceTetris::clearLines() {
  // Generate a bitmask with all 1s where we expect a peice.
  uint16_t mask = (1 << BOARD_WIDTH) - 1;
  for (uint8_t iy = BOARD_HEIGHT - 1; iy != 0; --iy) {
    if (board[iy] == mask) {
      // CLAER!
      score += 100;

      // Shift down lines
      for (uint8_t jy = iy; jy != 0; --jy) {
        board[jy] = board[jy - 1];
      }
      board[0] = 0;

      // Check the same line next iteration
      iy += 1;
    }
  }
}

// The AI.
void ClockfaceTetris::decideMove() {
  uint8_t bestDepth = 0;
  uint8_t bestX = 0;
  uint8_t bestR = 0;
  for (uint8_t r = 0; r < 4; ++r) {
	  for (int8_t i = 0; i < BOARD_WIDTH; ++i) {
		for (int8_t iy = 1; iy < BOARD_HEIGHT; ++iy) {
		  if (checkCollision(i - x, iy, r)) {
		    if (iy > bestDepth) {
		      bestDepth = iy;
		      bestX = i;
		      bestR = r;
		    }
		    break;
		  }
		}
	  }
  }
  targetX = bestX;
  targetRotation = bestR;
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
    uint8_t row = pgm_read_byte(gfx + (peice * 4 + rotation) * PEICE_MAX_HEIGHT + iy);
    for (uint8_t ix=0; ix < PEICE_MAX_WIDTH; ++ix) {
      if (row & _BV(ix)) {
        display->fillRect((ix + x) * 3 + BOARD_OFFSET_X, (iy + y) * 3, 2, 2, WHITE);
      }
    }
  }

  // Text
  display->setTextColor(WHITE);
  display->setCursor(56, 0);
  display->setTextSize(2);
  display->print(F("TETRIS"));

  // Score
  display->setTextSize(1);
  display->setCursor(55, 20);
  display->print(F("Score:"));
  display->print(score);
  display->setCursor(55, 28);
  display->print(F("High: "));
  display->print(highscore);

  // Time
  display->setCursor(55, 45);
  display->setTextSize(1);
  display->print(state.getDayStr(state.now.dayOfWeek()));
  display->print(F("  "));
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
  display->setCursor(55, 54);
  display->print(state.now.day());
  display->print(' ');
  display->print(state.getMonthStr(state.now.month()));
  display->print(' ');
  display->print(state.now.year());
}

