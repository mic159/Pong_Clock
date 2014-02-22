#include <Arduino.h>
#include <EEPROM.h>
#include "State.h"

// Grab RTC instance from .ino
extern RTC_DS1307 RTC;

State::State()
: now(0)
, timeLastUpdated(0)
, timeUpdated(false)
, timeMinuteUpdated(false)
, mode24h(false)
, dim(false)
{
  byte check = EEPROM.read(0);
  if (check == 0x41) {
    byte flags = EEPROM.read(1);
    mode24h = flags & _BV(0);
    dim = flags & _BV(1);
  }
}

void State::save() {
  EEPROM.write(0, 0x41);
  byte flags = 0;
  if (mode24h)
    flags |= _BV(0);
  if (dim)
    flags |= _BV(1);
  EEPROM.write(1, flags);
}

void State::update() {
  if (millis() - timeLastUpdated > 1000 || timeLastUpdated == 0) {
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

