#if !defined(SYSSTUFF_H)
#define SYSSTUFF_H
#if !defined(TEENSYDUINO)
#include <bluefruit.h>
#undef min
#undef max
#else
// Put the Teensy headers in here
#include <core_pins.h>
#include <keylayouts.h>
#include <stddef.h>
#include <usb_serial.h>
#endif
#endif
