#if !defined(DISP_GFX)
#define DISP_GLOBALS_H

#include "boardio.h"

#if defined(HAS_DISPLAY)

#if defined(HAS_BATTERY)
void drawBattery(uint8_t rate, uint8_t x, uint8_t y);
#endif

enum SpriteIndex {
  MACOS,
  WINDOWS,
  LINUX,
  FUNCTION,
  BLUETOOTH,
  SHIFT,
  CONTROL,
  OPTION,
  COMMAND,
  MODES,
  MODS
};

void drawSprite(SpriteIndex which, uint8_t x, uint8_t y);
void drawSwitches(const BoardIO::bits& matrix, uint8_t xo, uint8_t yo);

#endif
#endif