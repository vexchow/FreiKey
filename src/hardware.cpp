#include "hardware.h"
#include "battery.h"
#include "boardio.h"
#include "dbgcfg.h"
#include "debounce.h"

#if defined(HAS_BATTERY)
#define BATTERY_LEVEL(a) , battery_level(a)
#else
#define BATTERY_LEVEL(a)
#endif

namespace state {

#if defined(DEBUG)
uint32_t scans_since_last_time = 0;
#endif

hw::hw(uint8_t bl) : switches() BATTERY_LEVEL(bl) {}

hw::hw(uint32_t now, const hw& prev, const BoardIO& pd)
    : switches(prev.switches)
          BATTERY_LEVEL(readBattery(now, prev.battery_level)) {
  readSwitches(pd, now);
}

hw::hw(const hw& c) : switches(c.switches) BATTERY_LEVEL(c.battery_level) {}

void hw::readSwitches(const BoardIO& pd, uint32_t now) {
#if defined(DEBUG)
  scans_since_last_time++;
#endif
  // Read & debounce the current key matrix
  this->switches = debounce(pd.Read(), now);
}

#if defined(ADAFRUIT)

hw::hw(BLEClientUart& clientUart, const hw& prev) {
  if (!receive(clientUart, prev))
    memcpy(reinterpret_cast<uint8_t*>(this),
           reinterpret_cast<const uint8_t*>(&prev),
           sizeof(hw));
}

// Send the relevant data over the wire
void hw::send(BLEUart& bleuart, const hw& prev) const {
  bleuart.write((uint8_t*)&switches, sizeof(switches) + 1);
}

// Try to receive any relevant switch data from the wire.
// Returns true if something was received
bool hw::receive(BLEClientUart& clientUart, const hw& prev) {
  if (clientUart.available()) {
    uint8_t buffer[sizeof(switches) + 1];
    int size = clientUart.read(buffer, sizeof(switches) + 1);
    if (size == sizeof(switches) + 1) {
      memcpy(reinterpret_cast<uint8_t*>(this), buffer, sizeof(switches) + 1);
    } else {
#if defined(DEBUG)
      // NOTE: This fires occasionally when button mashing on the left, so
      // perhaps I shouldn't have changed this just on a whim. Wez clearly
      // knew what he was doing :)
      Serial.print("Incorrect datagram size:");
      Serial.print(" expected ");
      Serial.print(static_cast<uint8_t>(sizeof(hw)));
      Serial.print(" got ");
      Serial.println(size);
#endif
    }
    return true;
  }
  return false;
}

#endif

bool hw::operator==(const hw& o) const {
  return
#if defined(HAS_BATTERY)
      o.battery_level == battery_level &&
#endif
      o.switches == switches;
}

bool hw::operator!=(const hw& o) const {
  return !((*this) == o);
}

#if defined(DEBUG)
void hw::dump() const {
#if defined(HAS_BATTERY)
  dumpVal(battery_level, "Battery Level:");
#endif
  dumpHex(switches, "Integer value: ");
  Serial.println("");
  for (int64_t r = 0; r < BoardIO::numrows; r++) {
    for (int64_t c = 0; c < BoardIO::numcols; c++) {
      uint64_t mask = 1ULL << (r * BoardIO::numcols + c);
      if (switches.get_bit(r * BoardIO::numcols + c)) {
        Serial.print("X ");
      } else {
        Serial.print("- ");
      }
    }
    Serial.println("");
  }
}
#endif
} // namespace state

void rtos_idle_callback(void) {}
