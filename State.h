#ifndef STATE_H
#define STATE_H

#include <RTClib.h>

struct State {
  State();
  void update();
  void save();

  // The time
  DateTime now;
  bool timeUpdated;       // If the time was updated this frame
  bool timeMinuteUpdated; // Did the minute change this frame
  const __FlashStringHelper* getMonthStr(uint8_t m) const;
  const __FlashStringHelper* getDayStr(uint8_t d) const;

  // Settings
  bool mode24h;
  bool dim;

private:
  unsigned long timeLastUpdated;
};

extern State state;

#endif
