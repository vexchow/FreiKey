#include "hardware.h"

namespace state {
constexpr uint8_t VBAT_PIN = 31; // pin 31 is available for sampling the battery
// I had this set to 15, but still sometimes saw bounces :/
constexpr uint8_t DEBOUNCE_COUNT = 24;

// Some globals used by both halves
uint32_t last_bat_time = 0;
uint8_t stableCount = 0;
uint64_t recordedChange;

uint8_t getBatteryPercent() {
  float measuredvbat = analogRead(VBAT_PIN) * 6.6 / 1024;
  uint8_t bat_percentage = (uint8_t)round((measuredvbat - 3.7) * 200);
  return min(bat_percentage, 100);
}

uint8_t readBattery(uint32_t now, uint8_t prev) {
  if (prev && now - last_bat_time <= 30000) {
    // There's a lot of variance in the reading, so no need to over-report it.
    return prev;
  }
  uint8_t bat_percentage = getBatteryPercent();
  last_bat_time = now;
  DBG(dumpVal(bat_percentage, "Battery level: "));
  return bat_percentage;
}

hw::hw(uint8_t bl) : switches(0), battery_level(bl) {}

hw::hw(uint32_t now, const hw& prev, const PinData& pd)
    : switches(prev.switches),
      battery_level(readBattery(now, prev.battery_level)) {
  readSwitches(pd);
}

hw::hw(BLEClientUart& clientUart, const hw& prev) {
  if (!receive(clientUart, prev))
    memcpy(reinterpret_cast<uint8_t*>(this),
           reinterpret_cast<const uint8_t*>(&prev),
           sizeof(hw));
}

hw::hw(const hw& c) : switches(c.switches), battery_level(c.battery_level) {}

void hw::readSwitches(const PinData& pd) {
  uint64_t newSwitches = 0;
  for (uint64_t colNum = 0; colNum < numcols; ++colNum) {
    uint64_t val = 1ULL << colNum;
    digitalWrite(pd.cols[colNum], LOW);
    for (uint64_t rowNum = 0; rowNum < numrows; ++rowNum) {
      if (!digitalRead(pd.rows[rowNum])) {
        newSwitches |= val << (rowNum * numcols);
      }
    }
    digitalWrite(pd.cols[colNum], HIGH);
  }
  // Debouncing: This just waits for stable DEBOUNCE_COUNT reads before
  // reporting
  if (newSwitches != (stableCount ? recordedChange : switches)) {
    // We've observed a change (in either the delta or the reported: doesn't
    // matter which) record the change and (re)start the timer
    DBG2(if (stableCount) dumpVal(stableCount, "Resetting Debouncer: "));
    stableCount = 1;
    recordedChange = newSwitches;
  } else if (stableCount > DEBOUNCE_COUNT) {
    // We've had a stable reading for long enough: report it & clear the count
    switches = newSwitches;
    stableCount = 0;
  } else if (stableCount) {
    stableCount++;
  }
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
#if DEBUG
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

bool hw::operator==(const hw& o) const {
  return o.battery_level == battery_level && o.switches == switches;
}

bool hw::operator!=(const hw& o) const {
  return !((*this) == o);
}

#if DEBUG
void hw::dump() const {
  Serial.print("Battery Level:");
  Serial.println(battery_level);
  Serial.print("Integer value:");
  Serial.print(static_cast<unsigned int>(switches >> 32), 16);
  Serial.print("|");
  Serial.println(static_cast<unsigned int>(switches), 16);
  for (int64_t r = 0; r < numrows; r++) {
    for (int64_t c = numcols - 1; c >= 0; c--) {
      uint64_t mask = 1ULL << (r * numcols + c);
      if (switches & mask) {
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