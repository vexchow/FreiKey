#include "sysstuff.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <SPI.h>
#include <Wire.h>

#include "boardio.h"
#include "disp_gfx.h"

#if !defined(HAS_DISPLAY)
#error "Don't use this file if you don't have a display."
#endif

#if defined(HAS_BATTERY)
void drawBattery(uint8_t rate, uint8_t x, uint8_t y) {
  bool charge = (rate > 100);
  rate = rate - (charge ? 100 : 0);
  bool error = (rate > 100);
  display.drawRect(x, y, 30, 18, WHITE);
  display.drawRect(x + 30, y + 6, 2, 6, WHITE);
  if (!error) {
    display.fillRect(x + 2, y + 2, 26 * rate / 100, 14, WHITE);
    if (charge) {
      for (uint32_t i = 0; i < 4; i++) {
        display.drawFastHLine(x + 15 - i * 3, y + i + 4, i * 3 + 2, INVERSE);
        display.drawFastHLine(x + 14, y - i + 12, i * 3 + 2, INVERSE);
      }
      // Draw the middle line
      // If I did this in the above loop, it has a few
      // double inversion pixels
      display.drawFastHLine(x + 3, y + 8, 25, INVERSE);
    }
  } else {
    display.setCursor(x + 6, y + 5);
    display.setTextColor(INVERSE);
    display.print("???");
  }
}
#endif

void drawWin(uint8_t x, uint8_t y) {
  BoardIO::display->fillRect(x, y + 3, 5, 15, WHITE);
  BoardIO::display->fillRect(x + 5, y + 2, 5, 17, WHITE);
  BoardIO::display->fillRect(x + 11, y + 1, 5, 19, WHITE);
  BoardIO::display->fillRect(x + 16, y, 5, 21, WHITE);
  BoardIO::display->drawFastHLine(x, y + 10, 21, BLACK);
}

constexpr uint8_t apple[] = {
    B00000000, B00010000, //
    B00000000, B00110000, //
    B00000000, B01110000, //
    B00000000, B11100000, //
    B00000000, B11000000, //
    B00000000, B10000000, //
    B00011110, B00111100, //
    B00111111, B11111110, //
    B01111111, B11111111, //
    B01111111, B11111110, //
    B11111111, B11111100, //
    B11111111, B11111100, //
    B11111111, B11111100, //
    B11111111, B11111100, //
    B01111111, B11111110, //
    B01111111, B11111111, //
    B01111111, B11111111, //
    B00111111, B11111111, //
    B00111111, B11111110, //
    B00011111, B11111110, //
    B00001110, B00111000 //
};

constexpr uint8_t linux[] = {
    B00000000, B00000000, B11100000, //
    B00000000, B00000001, B11110000, //
    B00000000, B00011101, B11110000, //
    B00000000, B01111101, B11110000, //
    B00000001, B10111100, B11100000, //
    B00000011, B10111110, B00000000, //
    B00000111, B11010011, B11110000, //
    B00000111, B11000000, B11110000, //
    B01110011, B10000000, B01111000, //
    B11111011, B00000000, B01111000, //
    B11111011, B00000000, B00000000, //
    B11111011, B00000000, B01111000, //
    B01110011, B10000000, B01111000, //
    B00000111, B11000000, B11110000, //
    B00000111, B11010011, B11110000, //
    B00000011, B10111110, B00000000, //
    B00000001, B10111100, B11100000, //
    B00000000, B01111101, B11110000, //
    B00000000, B00011101, B11110000, //
    B00000000, B00000001, B11110000, //
    B00000000, B00000000, B11100000 //

};

constexpr uint8_t func[] = {
    B00001100, B00000000, //
    B00011100, B00000000, //
    B00111000, B00000000, //
    B00110000, B00000000, //
    B00110000, B00000000, //
    B00110000, B00000000, //
    B00110000, B00000000, //
    B00110000, B00000000, //
    B11111101, B11011100, //
    B11111101, B11111110, //
    B00110001, B11100111, //
    B00110001, B10000011, //
    B00110001, B10000011, //
    B00110001, B10000011, //
    B00110001, B10000011, //
    B00110001, B10000011, //
    B00110001, B10000011, //
    B00110001, B10000011, //
    B00110001, B10000011, //
    B00110001, B10000011, //
    B00110001, B10000011 //
};

constexpr uint8_t blueicon[] = {
    B00000110, B00000000, //
    B00000111, B00000000, //
    B00000111, B10000000, //
    B00000110, B11000000, //
    B01100110, B01100000, //
    B00110110, B11000000, //
    B00011111, B10000000, //
    B00001111, B00000000, //
    B00011111, B10000000, //
    B00110110, B11000000, //
    B01100110, B01100000, //
    B00000110, B11000000, //
    B00000111, B10000000, //
    B00000111, B00000000, //
    B00000110, B00000000 //
};

constexpr uint8_t capsicon[] = {
    B00000001, B10000000, //
    B00000011, B11000000, //
    B00000111, B11100000, //
    B00001111, B11110000, //
    B00011111, B11111000, //
    B00111111, B11111100, //
    B01111111, B11111110, //
    B11111111, B11111111, //
    B11111111, B11111111, //
    B00001111, B11110000, //
    B00001111, B11110000, //
    B00001111, B11110000, //
    B00001111, B11110000, //
    B00000000, B00000000, //
    B00001111, B11110000, //
    B00001111, B11110000 //
};

constexpr uint8_t shifticon[] = {
    B00000001, B10000000, //
    B00000011, B11000000, //
    B00000110, B01100000, //
    B00001100, B00110000, //
    B00011000, B00011000, //
    B00110000, B00001100, //
    B01100000, B00000110, //
    B11111100, B00111111, //
    B11111100, B00111111, //
    B00001100, B00110000, //
    B00001100, B00110000, //
    B00001100, B00110000, //
    B00001100, B00110000, //
    B00001100, B00110000, //
    B00001100, B00110000, //
    B00001111, B11110000 //
};

constexpr uint8_t ctrlicon[] = {
    B00000011, B00000000, //
    B00000111, B10000000, //
    B00001100, B11000000, //
    B00011000, B01100000, //
    B00110000, B00110000, //
    B01100000, B00011000, //
    B11000000, B00001100 //
};

constexpr uint8_t opticon[] = {
    B11111100, B00111111, //
    B00001100, B00000000, //
    B00000110, B00000000, //
    B00000110, B00000000, //
    B00000011, B00000000, //
    B00000011, B00000000, //
    B00000001, B10000000, //
    B00000001, B10000000, //
    B00000000, B11000000, //
    B00000000, B11000000, //
    B00000000, B01100000, //
    B00000000, B01111111 //
};

constexpr uint8_t cmdicon[] = {
    B00111000, B00011100, //
    B01111100, B00111110, //
    B11000110, B01100011, //
    B11000110, B01100011, //
    B11000110, B01100011, //
    B01111111, B11111110, //
    B00111111, B11111100, //
    B00000110, B01100000, //
    B00000110, B01100000, //
    B00111111, B11111100, //
    B01111111, B11111110, //
    B11000110, B01100011, //
    B11000110, B01100011, //
    B11000110, B01100011, //
    B01111100, B00111110, //
    B00111000, B00011100 //
};

constexpr uint8_t modestxt[] = {
    B11111110, B00111001, B11100111, B10011110, //
    B11011011, B01101101, B10110110, B00110000, //
    B11011011, B01101101, B10110111, B00011100, //
    B11011011, B01101101, B10110110, B00000110, //
    B11000011, B00111001, B11100111, B10111100 //
};

constexpr uint8_t modstxt[] = {
    B11111110, B00111001, B11100011, B11000000, //
    B11011011, B01101101, B10110110, B00000000, //
    B11011011, B01101101, B10110011, B10000000, //
    B11011011, B01101101, B10110000, B11000000, //
    B11000011, B00111001, B11100111, B10000000 //
};

 constexpr uint8_t bflogo[] = {
   B01111001, B11101111, B01111011, B11011110, 0, //
   B01101101, B10000110, B00110011, B00011011, 0, //
   B01111001, B11000110, B00110011, B10011110, 0, //
   B01101101, B10000110, B00110011, B00011011, 0, //
   B01111001, B11100110, B00110011, B11011011, 0, //
   0, 0, 0, 0, 0, //
   0, 0, 0, 0, 0, //
   B00000000, B00100000, B00000010, B00000000, 0, //
   B00000000, B00100000, B00000010, B00000000, 0, //
   B00000000, B00010000, B00000100, B00000000, 0, //
   B00000000, B00001000, B00001000, B00000000, 0, //
   B01110000, B00000100, B0001000, B00000111, 0, //
   B11111110, B00000011, B01100000, B00111111, 0x80, //
   B11111111, B11100111, B11110011, B11111111, 0x80, //
   B11000000, B00111111, B11111110, B00000001, 0x80, //
   B01100110, B00001111, B11111000, B00110011, 0, //
   B00110000, B11111111, B11111111, B10000110, 0, //
   B00011111, B11000011, B11100001, B11111100, 0, //
   B00001100, B00011001, B11001100, B00011000, 0, //
   B00001100, B00111001, B11001110, B00011000, 0, //
   B00000110, B01100011, B11100011, B00110000, 0, //
   B00000011, B00000110, B10110000, B01100000, 0, //
   B00000011, B00000110, B10110000, B01100000, 0, //
   B00000011, B00011100, B00011100, B01100000, 0, //
   B00000001, B11110000, B00000111, B11000000, 0, //
   0, 0, 0, 0, 0, //
   0, 0, 0, 0, 0, //
   B00000000, B11110110, B00110011, B00000000, 0, //
   B00000000, B11000110, B00110011, B00000000, 0, //
   B00000000, B11100110, B00011110, B00000000, 0, //
   B00000000, B11000110, B00001100, B00000000, 0, //
   B00000000, B11000111, B10001100, B00000000, 0 //
 };

struct sprite {
  union {
    void (*render)(uint8_t x, uint8_t y);
    const uint8_t* buffer;
  };
  uint8_t w, h, xo, yo;

  constexpr sprite(void (*r)(uint8_t x, uint8_t o))
      : render(r), w(0), h(0), xo(0), yo(0) {}
  constexpr sprite(
      const uint8_t* buf, uint8_t w, uint8_t h, uint8_t xo = 0, uint8_t yo = 0)
      : buffer(buf), w(w), h(h), xo(xo), yo(yo) {}
  void draw(uint8_t x, uint8_t y) const {
    if (w) {
      // This makes the white pixels white, and the black pixels are left alone
      BoardIO::display->fillRect(x + xo, y + yo, w, h, INVERSE);
      BoardIO::display->drawBitmap(
          x + xo, y + yo, buffer, w, h, WHITE, INVERSE);
    } else {
      render(x, y);
    }
  }
};

constexpr sprite sprites[] = {sprite(&apple[0], 16, 21, 1),
                              sprite(drawWin),
                              sprite(&linux[0], 21, 21),
                              sprite(&func[0], 16, 21),
#if defined(ADAFRUIT)
                              sprite(&blueicon[0], 11, 15),
#endif
                              sprite(&capsicon[0], 16, 16),
                              sprite(&shifticon[0], 16, 16),
                              sprite(&ctrlicon[0], 14, 7),
                              sprite(&opticon[0], 16, 12),
                              sprite(&cmdicon[0], 16, 16),
                              sprite(&modestxt[0], 31, 5),
                              sprite(&modstxt[0], 26, 5),
                              sprite(&bflogo[0], 33, 32)};

void drawSprite(SpriteIndex lyr, uint8_t x, uint8_t y) {
  sprites[lyr % (sizeof(sprites) / sizeof(sprites[0]))].draw(x, y);
}

#if defined(TEENSY)
constexpr uint8_t keyWidth = 3;
constexpr uint8_t keyHeight = 3;
constexpr uint8_t splitSpace = 1;
constexpr uint8_t outline = 0;
void drawSwitches(const BoardIO::bits& matrix, uint8_t xo, uint8_t yo) {
  for (uint8_t x = 0; x < BoardIO::ncols; x++) {
    uint8_t xx = (x >= 6) ? splitSpace : 0;
    for (uint8_t y = 0; y < BoardIO::nrows; y++) {
      uint8_t yy = (false && (x == 5 || x == 6)) ? (y == 5) : 0;
      xx = yy ? (xx + (x == 5) - (x == 6)) : xx;
      if (!((x == 0 || x == 11) && y == 5)) { // Don't draw the 'missing' keys
        BoardIO::display->drawRect(xo + x * keyWidth + xx,
                                   yo + y * keyHeight + yy,
                                   keyWidth + 1,
                                   keyHeight + 1,
                                   WHITE);
        if (matrix.get_bit(x + y * BoardIO::ncols)) {
          // If it's pressed, fill it in
          BoardIO::display->fillRect(xo + x * keyWidth + outline + xx,
                                     yo + y * keyHeight + outline + yy,
                                     keyWidth - (2 * outline - 1),
                                     keyHeight - (2 * outline - 1),
                                     WHITE);
        }
      }
    }
  }
}
#endif
