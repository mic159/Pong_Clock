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

  // Settings
  bool mode24h;
  bool dim;

private:
  unsigned long timeLastUpdated;
};

extern State state;

#endif
