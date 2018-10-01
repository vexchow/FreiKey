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
    B00000001, B10000000, //
    B00000011, B11000000, //
    B00000110, B01100000, //
    B00001100, B00110000, //
    B00011000, B00011000, //
    B00110000, B00001100, //
    B01100000, B00000110 //
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
 B11110001, B00110011, B10011100, //
 B10101010, B10101010, B00100000, //
 B10101010, B10101011, B00011000, //
 B10101010, B10101010, B00000100, //
 B10001001, B00110011, B10111000 //
};

constexpr uint8_t modstxt[] = {
 B11110001, B00110001, B11000000, //
 B10101010, B10101010, B00000000, //
 B10101010, B10101001, B10000000, //
 B10101010, B10101000, B01000000, //
 B10001001, B00110011, B10000000 //
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
      // This makes the white pixels white, and the black pixels are left alone
      BoardIO::display->fillRect(x+xo,y+yo,w,h,INVERSE);
      BoardIO::display->drawBitmap(x + xo, y + yo, buffer, w, h, WHITE, INVERSE);
    } else {
      render(x, y);
    }
  }
};

sprite sprites[] = {sprite(&apple[0], 16, 21, 1),
                    sprite(drawWin),
                    sprite(&linux[0], 21, 21),
                    sprite(&func[0], 16, 21),
                    sprite(&blueicon[0], 11, 15),
                    sprite(&shifticon[0], 16, 16),
                    sprite(&ctrlicon[0], 16, 7),
                    sprite(&opticon[0], 16, 12),
                    sprite(&cmdicon[0], 16, 16),
                    sprite(&modestxt[0], 22, 5),
                    sprite(&modstxt[0], 18, 5)};

void drawSprite(SpriteIndex lyr, uint8_t x, uint8_t y) {
  sprites[lyr % (sizeof(sprites) / sizeof(sprites[0]))].draw(x, y);
}

/*********************************************************************
Written by Limor Fried/Ladyada  for Adafruit Industries.
BSD license, check license.txt for more information
All text above, and the splash screen must be included in any redistribution
*********************************************************************/

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

#if 0
void testdrawbitmap(const uint8_t *bitmap, uint8_t w, uint8_t h) {
  // initialize
  for (uint8_t f=0; f< NUMFLAKES; f++) {
    icons[f][XPOS] = random(display.width());
    icons[f][YPOS] = 0;
    icons[f][DELTAY] = random(5) + 1;

    Serial.print("x: ");
    Serial.print(icons[f][XPOS], DEC);
    Serial.print(" y: ");
    Serial.print(icons[f][YPOS], DEC);
    Serial.print(" dy: ");
    Serial.println(icons[f][DELTAY], DEC);
  }
}


void testdrawchar(void) {
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);

  for (uint8_t i=0; i < 168; i++) {
    if (i == '\n') continue;
    display.write(i);
    if ((i > 0) && (i % 21 == 0))
      display.println();
  }
  display.display();
  delay(1);
}

void testdrawcircle(void) {
  for (int16_t i=0; i<display.height(); i+=2) {
    display.drawCircle(display.width()/2, display.height()/2, i, WHITE);
    display.display();
    delay(1);
  }
}

void testfillrect(void) {
  uint8_t color = 1;
  for (int16_t i=0; i<display.height()/2; i+=3) {
    // alternate colors
    display.fillRect(i, i, display.width()-i*2, display.height()-i*2, color%2);
    display.display();
    delay(1);
    color++;
  }
}

void testdrawtriangle(void) {
  for (int16_t i=0; i<min(display.width(),display.height())/2; i+=5) {
    display.drawTriangle(display.width()/2, display.height()/2-i,
                     display.width()/2-i, display.height()/2+i,
                     display.width()/2+i, display.height()/2+i, WHITE);
    display.display();
    delay(1);
  }
}

void testfilltriangle(void) {
  uint8_t color = WHITE;
  for (int16_t i=min(display.width(),display.height())/2; i>0; i-=5) {
    display.fillTriangle(display.width()/2, display.height()/2-i,
                     display.width()/2-i, display.height()/2+i,
                     display.width()/2+i, display.height()/2+i, WHITE);
    if (color == WHITE) color = BLACK;
    else color = WHITE;
    display.display();
    delay(1);
  }
}

void testdrawroundrect(void) {
  for (int16_t i=0; i<display.height()/2-2; i+=2) {
    display.drawRoundRect(i, i, display.width()-2*i, display.height()-2*i, display.height()/4, WHITE);
    display.display();
    delay(1);
  }
}

void testfillroundrect(void) {
  uint8_t color = WHITE;
  for (int16_t i=0; i<display.height()/2-2; i+=2) {
    display.fillRoundRect(i, i, display.width()-2*i, display.height()-2*i, display.height()/4, color);
    if (color == WHITE) color = BLACK;
    else color = WHITE;
    display.display();
    delay(1);
  }
}

void testdrawrect(void) {
  for (int16_t i=0; i<display.height()/2; i+=2) {
    display.drawRect(i, i, display.width()-2*i, display.height()-2*i, WHITE);
    display.display();
    delay(1);
  }
}

void testdrawline() {
  for (int16_t i=0; i<display.width(); i+=4) {
    display.drawLine(0, 0, i, display.height()-1, WHITE);
    display.display();
    delay(1);
  }
  for (int16_t i=0; i<display.height(); i+=4) {
    display.drawLine(0, 0, display.width()-1, i, WHITE);
    display.display();
    delay(1);
  }
  delay(250);

  display.clearDisplay();
  for (int16_t i=0; i<display.width(); i+=4) {
    display.drawLine(0, display.height()-1, i, 0, WHITE);
    display.display();
    delay(1);
  }
  for (int16_t i=display.height()-1; i>=0; i-=4) {
    display.drawLine(0, display.height()-1, display.width()-1, i, WHITE);
    display.display();
    delay(1);
  }
  delay(250);

  display.clearDisplay();
  for (int16_t i=display.width()-1; i>=0; i-=4) {
    display.drawLine(display.width()-1, display.height()-1, i, 0, WHITE);
    display.display();
    delay(1);
  }
  for (int16_t i=display.height()-1; i>=0; i-=4) {
    display.drawLine(display.width()-1, display.height()-1, 0, i, WHITE);
    display.display();
    delay(1);
  }
  delay(250);

  display.clearDisplay();
  for (int16_t i=0; i<display.height(); i+=4) {
    display.drawLine(display.width()-1, 0, 0, i, WHITE);
    display.display();
    delay(1);
  }
  for (int16_t i=0; i<display.width(); i+=4) {
    display.drawLine(display.width()-1, 0, i, display.height()-1, WHITE);
    display.display();
    delay(1);
  }
  delay(250);
}

void testscrolltext(void) {
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(10,0);
  display.clearDisplay();
  display.println("scroll");
  display.display();
  delay(1);

  display.startscrollright(0x00, 0x0F);
  delay(2000);
  display.stopscroll();
  delay(1000);
  display.startscrollleft(0x00, 0x0F);
  delay(2000);
  display.stopscroll();
  delay(1000);
  display.startscrolldiagright(0x00, 0x07);
  delay(2000);
  display.startscrolldiagleft(0x00, 0x07);
  delay(2000);
  display.stopscroll();
}
#endif
