#if !defined(DISP_GFX)
#define DISP_GLOBALS_H

#if defined(HAS_DISPLAY)

#include <Adafruit_SSD1306.h>

extern Adafruit_SSD1306 display;

#if defined(HAS_BATTERY)
void drawBattery(uint8_t rate, uint8_t x, uint8_t y);
#endif

void drawSprite(uint8_t lyr, uint8_t x, uint8_t y);

#endif
#endif