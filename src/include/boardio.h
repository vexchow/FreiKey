#if !defined(BOARDIO_H)
#define BOARDIO_H

#include "bit_array.h"
#include "sysstuff.h"

class BoardIO {
 public:
#if defined(TEENSY)
  static const uint8_t ncols = 12;
#elif defined(ADAFRUIT)
  static const uint8_t ncols = 7;
#endif
  static const uint8_t nrows = 6;
  static const uint8_t matrix_size = ncols * nrows;
  typedef bit_array<matrix_size> bits;

  uint8_t cols[ncols];
  uint8_t rows[nrows];
#if defined(HAS_LED)
  uint8_t led;
  void setLED(uint32_t brightness) const;
#endif
#if defined(HAS_BATTERY)
  static uint8_t getBatteryPercent();
#endif
#if defined(HAS_DISPLAY)
  static class Adafruit_SSD1306 *display;
#endif
  void Configure() const;
  bits Read() const;
};

#endif
