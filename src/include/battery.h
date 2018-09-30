#if !defined(BATTERY_H)
#define BATTERY_H
#if defined(HAS_BATTERY)

#include <stdint.h>

uint8_t readBattery(uint32_t now, uint8_t prev);

#endif
#endif
