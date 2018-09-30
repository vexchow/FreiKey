#if !defined(SYSSTUFF_H)
#define SYSSTUFF_H
#if defined(ADAFRUIT)
#include <bluefruit.h>
#undef min
#undef max
#define HAS_BATTERY
#elif defined(TEENSY)
// Put the Teensy headers in here
#include "core_pins.h"
#include "usb_keyboard.h"
#include "usb_serial.h"
#include <stddef.h>
#define HAS_DISPLAY 1
#endif
#endif
