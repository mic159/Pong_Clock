#include <Arduino.h>
#include "State.h"

// Grab RTC instance from .ino
extern RTC_DS1307 RTC;

State::State()
: now(0)
, timeLastUpdated(0)
, timeUpdated(false)
, timeMinuteUpdated(false)
, mode24h(false)
{}

void State::update() {
  if (millis() - timeLastUpdated > 1000) {
    uint8_t minute = now.minute();
    now = RTC.now();
    timeLastUpdated = millis();
    timeMinuteUpdated = now.minute() != minute;
    timeUpdated = true;
  } else {
    timeMinuteUpdated = false;
    timeUpdated = false;
  }
}
