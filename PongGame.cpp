
#define WHITE 1

#include <arduino.h>
#include <Adafruit_GFX.h>
#include "Paddle.h"
#include "Ball.h"
#include "PongGame.h"



PongGame::PongGame()
: lpaddle(0, 0), rpaddle(128 - 2, 64 - 14)
, ball(2, 2, 1, 1)
, lscore(0), rscore(0)
, pause(0)
{
  lpaddle.y = random(0, 64 - lpaddle.h);
  ball.x = lpaddle.w;
  ball.y = lpaddle.y + (lpaddle.h / 2);
  ball.dx = 1;
  ball.calcTarget();
}

void PongGame::setScore(uint16_t hour, uint16_t minute) {
  lscore = minute;
  rscore = hour;
}

void PongGame::update(uint16_t hour, uint16_t minute) {
  // If paused, dont do any movements
  if (pause > 0) {
    --pause;
    return;
  }

  lpaddle.update(ball.target_y, ball.dx, lscore != minute);
  rpaddle.update(ball.target_y, ball.dx, rscore != hour);

  bool hit = ball.update(lpaddle, rpaddle);

  if (hit) {
    // Wohoo, a score!
    if (ball.x > 128 / 2) {
      lscore += 1;
    } else {
      rscore += 1;
    }
    // New game (ball on lpaddle)
    lpaddle.y = random(0, 64 - lpaddle.h);
    ball.x = lpaddle.w;
    ball.y = lpaddle.y + (lpaddle.h / 2);
    ball.dx = 1;
    ball.calcTarget();
    pause = 50;
  }
}

void PongGame::draw(Adafruit_GFX& display) const {
  lpaddle.draw(display);
  rpaddle.draw(display);
  // Middle line
  for(uint8_t i = 0; i < 64 / 4; i = i + 2) {
    display.fillRect(128 / 2 - 1, i * 4, 2, 4, WHITE);
  }
  drawScore(display);
  ball.draw(display);
}

void PongGame::drawScore(Adafruit_GFX& display) const {
  int lscore_x, rscore_x;
  static const int font_w = 10;
  static const int font_sp = 4;

  if (lscore > 9) lscore_x = 128 / 2 - font_w - font_w - font_sp - 10;
  else lscore_x = 128 / 2 - font_w - 10;

  rscore_x = 128 / 2 + 10;

  display.setTextColor(WHITE);

  display.setCursor(lscore_x, 4/2);
  display.println(lscore);

  display.setCursor(rscore_x, 4/2);
  display.println(rscore);
}

