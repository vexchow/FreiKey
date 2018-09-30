#include <Adafruit_SSD1306.h>

#include "disp_gfx.h"
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

void status::display() {
  drawBattery(leftBattery, 0, 7);
  drawBattery(rightBattery, 96, 7);
}
} // namespace disp_uart
