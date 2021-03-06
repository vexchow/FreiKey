#if !defined(STATE_HARDWARE_H)
#define STATE_HARDWARE_H

#include "sysstuff.h"

#include "boardio.h"
#include "dbgcfg.h"

using scancode_t = uint16_t;
constexpr scancode_t null_scan_code = 0xffff;

#if defined(ADAFRUIT)
constexpr char* MANUFACTURER = "FreikyStuff";
constexpr char* MODEL = "FreiKeyboard";
constexpr char* BT_NAME = "FreiKeys";
constexpr char* HW_REV = "0001";
constexpr char* LHS_NAME = "FreiKeys-Slave";
#endif

namespace state {

// This struct is to encapsulate the complete hardware state, including both
// which switches are down, as well as the current battery level.
struct hw {
  BoardIO::bits switches;
#if defined(HAS_BATTERY)
  uint8_t battery_level;
#endif
  // This is just a dump constructor
  hw(uint8_t bl = 0);

  // This is for reading the data from the hardware
  hw(uint32_t now, const hw& prev, const BoardIO& pd);

  // Generic copy constructor...
  hw(const hw& c);

  // Just reads the switches...
  void readSwitches(const BoardIO& pd, uint32_t now);

#if defined(ADAFRUIT)
  // This is for reading the data from the left hand side over the UART
  hw(BLEClientUart& clientUart, const hw& prev);

  // Send the relevant data over the wire
  void send(BLEUart& bleuart, const hw& prev) const;

  // Try to receive any relevant switch data from the wire.
  // Returns true if something was received
  bool receive(BLEClientUart& clientUart, const hw& prev);
#endif

  bool operator==(const hw& o) const;
  bool operator!=(const hw& o) const;

// A little helper for serial port dumping...
#if defined(DEBUG)
  void dump() const;
#endif
};
} // namespace state
#endif
