#ifndef UTIL_H
#define UTIL_H

#include <avr/pgmspace.h>

// To reduce SRAM usage, dont have all strings sitting in ram.
// Replace:
//   display.print("Hello World");
// With:
//   display_print(PSTR("Hello World"));
void display_print(const prog_char *str);

#endif
