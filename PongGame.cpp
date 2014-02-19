#include <Arduino.h>
#include <Adafruit_GFX.h>
#include "Paddle.h"
#include "Ball.h"
#include "PongGame.h"

#define WHITE 1
#define WIDTH 128
#define HEIGHT 64

PongGame::PongGame()
: lpaddle(0, 0), rpaddle(WIDTH - 2, HEIGHT - 14)
, ball(2, 2, 1, 1)
, lscore(0), rscore(0)
, pause(0)
{
  lpaddle.y = random(0, HEIGHT - lpaddle.h);
  ball.x = lpaddle.w;
  ball.y = lpaddle.y + (lpaddle.h / 2);
  ball.dx = 1;
  ball.calcTarget();
}

void PongGame::setScore(uint8_t hour, uint8_t minute) {
  lscore = hour;
  rscore = minute;
}

void PongGame::update(uint8_t hour, uint8_t minute) {
  // If paused, dont do any movements
  if (pause > 0) {
    --pause;
    return;
  }

  lpaddle.update(ball.target_y, ball.dx, rscore != minute);
  rpaddle.update(ball.target_y, ball.dx, lscore != hour);

  bool hit = ball.update(lpaddle, rpaddle);

  if (hit) {
    // Wohoo, a score!
    lscore = hour;
    rscore = minute;
    // New game (ball on lpaddle)
    lpaddle.y = random(0, HEIGHT - lpaddle.h);
    ball.x = lpaddle.w;
    ball.y = lpaddle.y + (lpaddle.h / 2);
    ball.dx = 1;
    ball.calcTarget();
    pause = 50;
  }
}

void PongGame::draw(Adafruit_GFX* display) const {
  lpaddle.draw(display);
  rpaddle.draw(display);
  // Middle line
  for(uint8_t i = 0; i < HEIGHT / 4; i = i + 2) {
    display->fillRect(WIDTH / 2 - 1, i * 4, 2, 4, WHITE);
  }
  drawScore(display);
  ball.draw(display);
}

void PongGame::drawScore(Adafruit_GFX* display) const {
  int lscore_x, rscore_x;
  static const int font_w = 10;
  static const int font_sp = 4;

  if (lscore > 9) lscore_x = WIDTH / 2 - font_w - font_w - font_sp - 10;
  else lscore_x = WIDTH / 2 - font_w - 10;

  rscore_x = WIDTH / 2 + 10;

  display->setTextSize(2);
  display->setTextColor(WHITE);

  display->setCursor(lscore_x, 4/2);
  display->println(lscore);

  display->setCursor(rscore_x, 4/2);
  display->println(rscore);
}

