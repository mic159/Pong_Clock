#ifndef STATE_H
#define STATE_H

#include <RTClib.h>

struct State {
  State();
  void update();

  // The time
  DateTime now;
  bool timeUpdated;       // If the time was updated this frame
  bool timeMinuteUpdated; // Did the minute change this frame

  // Settings
  bool mode24h;
  uint8_t brightness;

private:
  unsigned long timeLastUpdated;
};

extern State state;

#endif
