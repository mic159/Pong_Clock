#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "Util.h"

extern Adafruit_SSD1306 display;

void display_print(const prog_char *str) {
  char c;
  if (!str) return;
  while ((c = pgm_read_byte(str++))) {
    display.print(c);
  }
}
