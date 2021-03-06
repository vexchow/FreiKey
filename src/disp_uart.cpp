#include <Adafruit_SSD1306.h>

#include "disp_globals.h"
#include "disp_uart.h"

namespace disp_uart {

/*status status::create(const char *host, uint8_t lbat, uint8_t rbat,
    bool lchg, bool rchg, bool lstt, uint8_t base, uint8_t time) {
      uart::status res;
      res.leftBattery = lbat;
      res.rightBattery = rbat;
      res.leftCharging = lchg;
      res.rightCharging = rchg;
      res.leftStat = lsts;
      res.baselayer = base;
      strncpy(host, res.hostName[0], 32);
      res.hostName[31] = 0;
      return res;
    }*/

uint8_t encodeBatteryValue(uint8_t chargeRemaining,
                           bool isCharging,
                           bool isPresent) {
  return isPresent ? 0xFF : (chargeRemaining + (isCharging ? 100 : 0));
}

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

void drawWin(uint8_t x, uint8_t y) {
  display.fillRect(x, y + 3, 5, 15, WHITE);
  display.fillRect(x + 5, y + 2, 5, 17, WHITE);
  display.fillRect(x + 11, y + 1, 5, 19, WHITE);
  display.fillRect(x + 16, y, 5, 21, WHITE);
  display.drawFastHLine(x, y + 10, 21, BLACK);
}

constexpr uint8_t apple[] = {
    0x00, 0x10, // ---- ---- ---X ----
    0x00, 0x30, // ---- ---- --XX ----
    0x00, 0x70, // ---- ---- -XXX ----
    0x00, 0xE0, // ---- ---- XXX- ----
    0x00, 0xC0, // ---- ---- XX-- ----
    0x00, 0x80, // ---- ---- X--- ----
    0x1E, 0x3C, // ---X XXX- --XX XX--
    0x3F, 0xFE, // --XX XXXX XXXX XXX-
    0x7F, 0xFF, // -XXX XXXX XXXX XXXX
    0x7F, 0xFE, // -XXX XXXX XXXX XXX-
    0xFF, 0xFC, // XXXX XXXX XXXX XX--
    0xFF, 0xFC, // XXXX XXXX XXXX XX--
    0xFF, 0xFC, // XXXX XXXX XXXX XX--
    0xFF, 0xFC, // XXXX XXXX XXXX XX--
    0x7F, 0xFE, // -XXX XXXX XXXX XXX-
    0x7F, 0xFF, // -XXX XXXX XXXX XXXX
    0x7F, 0xFF, // -XXX XXXX XXXX XXXX
    0x3F, 0xFF, // --XX XXXX XXXX XXXX
    0x3F, 0xFE, // --XX XXXX XXXX XXX-
    0x1F, 0xFE, // ---X XXXX XXXX XXX-
    0x0E, 0x38 //  ---- XXX- --XX X---
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
    0x06, 0x00  // ---- -xx- ---
};
struct layer {
  union {
    void (*render)(uint8_t x, uint8_t y);
    const uint8_t* buffer;
  };
  uint8_t w, h, xo, yo;

  layer(void (*r)(uint8_t x, uint8_t o))
      : render(r), w(0), h(0), xo(0), yo(0) {}
  layer(const uint8_t* buf, uint8_t w, uint8_t h, uint8_t xo = 0, uint8_t yo = 0)
      : buffer(buf), w(w), h(h), xo(xo), yo(yo) {}
  void draw(uint8_t x, uint8_t y) {
    if (w) {
      display.drawBitmap(x + xo, y + yo, buffer, w, h, WHITE, BLACK);
    } else {
      render(x, y);
    }
  }
};

layer layers[] = {layer(&apple[0], 16, 21, 1),
                  layer(drawWin),
                  layer(&linux[0], 21, 21),
                  layer(&func[0], 16, 21),
                  layer(&blueicon[0], 11, 15)};

void drawLayer(uint8_t lyr, uint8_t x, uint8_t y) {
  layers[lyr].draw(x, y);
}

void status::display() {
  drawBattery(leftBattery, 0, 7);
  drawBattery(rightBattery, 96, 7);
}
} // namespace disp_uart
