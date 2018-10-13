#if !defined(DISP_GFX)
#define DISP_GLOBALS_H

#include "boardio.h"

#if defined(HAS_DISPLAY)

enum SpriteIndex {
  MACOS,
  WINDOWS,
  LINUX,
  FUNCTION,
#if defined(ADAFRUIT)
  BLUETOOTH,
#endif
  CAPS,
  SHIFT,
  CONTROL,
  OPTION,
  COMMAND,
  MODES,
  MODS,
  LOGO
};

void drawSprite(SpriteIndex which, uint8_t x, uint8_t y);
#if defined(TEENSY)
void drawSwitches(const BoardIO::bits& matrix, uint8_t xo, uint8_t yo);
#endif
#if defined(HAS_BATTERY)
void drawBattery(uint8_t rate, uint8_t x, uint8_t y);
#endif

#endif
#endif