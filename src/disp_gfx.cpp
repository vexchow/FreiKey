#include "sysstuff.h"

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
  display.fillRect(x, y + 3, 5, 15, WHITE);
  display.fillRect(x + 5, y + 2, 5, 17, WHITE);
  display.fillRect(x + 11, y + 1, 5, 19, WHITE);
  display.fillRect(x + 16, y, 5, 21, WHITE);
  display.drawFastHLine(x, y + 10, 21, BLACK);
}

constexpr uint8_t apple[] = {
  B00000000, B00010000,
  B00000000, B00110000,
  B00000000, B01110000,
  B00000000, B11100000,
  B00000000, B11000000,
  B00000000, B10000000,
  B00011110, B00111100,
  B00111111, B11111110,
  B01111111, B11111111,
  B01111111, B11111110,
  B11111111, B11111100,
  B11111111, B11111100,
  B11111111, B11111100,
  B11111111, B11111100,
  B01111111, B11111110,
  B01111111, B11111111,
  B01111111, B11111111,
  B00111111, B11111111,
  B00111111, B11111110,
  B00011111, B11111110,
  B00001110, B00111000
};

constexpr uint8_t linux[] = {
    0x00, 0x00, 0xE0, // ---- ---- ---- ---- XXX- -|||
    0x00, 0x01, 0xF0, // ---- ---- ---- ---X XXXX -|||
    0x00, 0x1D, 0xF0, // ---- ---- ---X XX-X XXXX -|||
    0x00, 0x7D, 0xF0, // ---- ---- -XXX XX-X XXXX -|||
    0x01, 0xBC, 0xE0, // ---- ---X X-XX XX-- XXX- -|||
    0x03, 0xBE, 0x00, // ---- --XX X-XX XXX- ---- -|||
    0x07, 0xD3, 0xF0, // ---- -XXX XX-X --XX XXXX -|||
    0x07, 0xC0, 0xF0, // ---- -XXX XX-- ---- XXXX -|||
    0x73, 0x80, 0x78, // -XXX --XX X--- ---- -XXX X|||
    0xFB, 0x00, 0x78, // XXXX X-XX ---- ---- -XXX X|||
    0xFB, 0x00, 0x00, // XXXX X-XX ---- ---- ---- -|||
    0xFB, 0x00, 0x78, // XXXX X-XX ---- ---- -XXX X|||
    0x73, 0x80, 0x78, // -XXX --XX X--- ---- -XXX X|||
    0x07, 0xC0, 0xF0, // ---- -XXX XX-- ---- XXXX -|||
    0x07, 0xD3, 0xF0, // ---- -XXX XX-X --XX XXXX -|||
    0x03, 0xBE, 0x00, // ---- --XX X-XX XXX- ---- -|||
    0x01, 0xBC, 0xE0, // ---- ---X X-XX XX-- XXX- -|||
    0x00, 0x7D, 0xF0, // ---- ---- -XXX XX-X XXXX -|||
    0x00, 0x1D, 0xF0, // ---- ---- ---X XX-X XXXX -|||
    0x00, 0x01, 0xF0, // ---- ---- ---- ---X XXXX -|||
    0x00, 0x00, 0xE0 //  ---- ---- ---- ---- XXX- -|||
};

constexpr uint8_t func[] = {
    0x0C, 0x00, // ---- XX-- ---- ----
    0x1C, 0x00, // ---X XX-- ---- ----
    0x38, 0x00, // --XX X--- ---- ----
    0x30, 0x00, // --XX ---- ---- ----
    0x30, 0x00, // --XX ---- ---- ----
    0x30, 0x00, // --XX ---- ---- ----
    0x30, 0x00, // --XX ---- ---- ----
    0x30, 0x00, // --XX ---- ---- ----
    0xFD, 0xBC, // XXXX XX-X X-XX XX--
    0xFD, 0xFE, // XXXX XX-X XXXX XXX-
    0x31, 0xE7, // --XX ---X XXX- -XXX
    0x31, 0xC3, // --XX ---X XX-- --XX
    0x31, 0x83, // --XX ---X X--- --XX
    0x31, 0x83, // --XX ---X X--- --XX
    0x31, 0x83, // --XX ---X X--- --XX
    0x31, 0x83, // --XX ---X X--- --XX
    0x31, 0x83, // --XX ---X X--- --XX
    0x31, 0x83, // --XX ---X X--- --XX
    0x31, 0x83, // --XX ---X X--- --XX
    0x31, 0x83, // --XX ---X X--- --XX
    0x31, 0x83, // --XX ---X X--- --XX
};

constexpr uint8_t blueicon[] = {
    0x06, 0x00, // ---- -xx- ---
    0x07, 0x00, // ---- -xxx ---
    0x07, 0x80, // ---- -xxx x--
    0x06, 0xC0, // ---- -xx- xx-
    0xC6, 0x60, // -xx- -xx- -xx
    0x36, 0xC0, // --xx -xx- xx-
    0x1F, 0x80, // ---x xxxx x--
    0x0F, 0x00, // ---- xxxx ---
    0x1F, 0x80, // ---x xxxx x--
    0x36, 0xC0, // --xx -xx- xx-
    0xC6, 0x60, // -xx- -xx- -xx
    0x06, 0xC0, // ---- -xx- xx-
    0x07, 0x80, // ---- -xxx x--
    0x07, 0x00, // ---- -xxx ---
    0x06, 0x00 // ---- -xx- ---
};
struct sprite {
  union {
    void (*render)(uint8_t x, uint8_t y);
    const uint8_t* buffer;
  };
  uint8_t w, h, xo, yo;

  sprite(void (*r)(uint8_t x, uint8_t o))
      : render(r), w(0), h(0), xo(0), yo(0) {}
  sprite(
      const uint8_t* buf, uint8_t w, uint8_t h, uint8_t xo = 0, uint8_t yo = 0)
      : buffer(buf), w(w), h(h), xo(xo), yo(yo) {}
  void draw(uint8_t x, uint8_t y) {
    if (w) {
      display.drawBitmap(x + xo, y + yo, buffer, w, h, WHITE, BLACK);
    } else {
      render(x, y);
    }
  }
};

sprite sprites[] = {sprite(&apple[0], 16, 21, 1),
                    sprite(drawWin),
                    sprite(&linux[0], 21, 21),
                    sprite(&func[0], 16, 21),
                    sprite(&blueicon[0], 11, 15)};

void drawLayer(uint8_t lyr, uint8_t x, uint8_t y) {
  sprites[lyr].draw(x, y);
}