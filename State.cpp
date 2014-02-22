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

const __FlashStringHelper* State::getMonthStr(uint8_t m) const {
  switch (m) {
    default:
    case 0: return F("Jan");
    case 1: return F("Feb");
    case 2: return F("Mar");
    case 3: return F("Apr");
    case 4: return F("May");
    case 5: return F("Jun");
    case 6: return F("Jul");
    case 7: return F("Aug");
    case 8: return F("Sep");
    case 9: return F("Oct");
    case 10: return F("Nov");
    case 11: return F("Dec");
  }
}

const __FlashStringHelper* State::getDayStr(uint8_t d) const {
  switch (d) {
    default:
    case 0: return F("Sun");
    case 1: return F("Mon");
    case 2: return F("Tue");
    case 3: return F("Wed");
    case 4: return F("Thu");
    case 5: return F("Fri");
    case 6: return F("Sat");
  }
}

