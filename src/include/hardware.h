#if !defined(STATE_HARDWARE_H)
#define STATE_HARDWARE_H

#include <bluefruit.h>

#include "dbgcfg.h"
#include "pindata.h"



// These numbers correspond to the *port pin* numbers in the nRF52 documentation
// not the physical pin numbers...
constexpr PinData LeftPins = {
    {15, 2, 16, 7, 30, 27, 11}, {5, 12, 13, 28, 4, 3}, 29};
constexpr PinData RightPins = {
    {29, 16, 15, 7, 27, 11, 30}, {13, 4, 2, 3, 5, 12}, 28};
using scancode_t = uint8_t;

constexpr char *MANUFACTURER = "FreikyStuff";
constexpr char *MODEL = "FreiKeyboard";
constexpr char *BT_NAME = "FreiKeys";
constexpr char *HW_REV = "0001";
constexpr char *LHS_NAME = "FreiKeys-Slave";

// Find last bit set in a long (BSD function, not available in Arduino)
inline uint8_t flsl(uint64_t val) {
  // GCC builtin function seems to work on Arduino :)
  return static_cast<uint8_t>(63 - __builtin_clzll(val));
}

namespace state {

void shared_setup(const PinData &pd);

// This struct is to encapsulate the complete hardware state, including both
// which switches are down, as well as the current battery level.
struct hw {
  uint64_t switches;
  uint8_t battery_level;

  // This is just a dump constructor
  hw(uint8_t bl = 0);

  // This is for reading the data from the hardware
  hw(uint32_t now, const hw& prev, const PinData& pd);

  // This is for reading the data from the left hand side over the UART
  hw(BLEClientUart& clientUart, const hw& prev);

  // Generic copy constructor...
  hw(const hw& c);

  // Just reads the switches...
  void readSwitches(const PinData& pd, uint32_t now);

  // Send the relevant data over the wire
  void send(BLEUart& bleuart, const hw& prev) const;

  // Try to receive any relevant switch data from the wire.
  // Returns true if something was received
  bool receive(BLEClientUart& clientUart, const hw& prev);

  bool operator==(const hw& o) const;
  bool operator!=(const hw& o) const;

// A little helper for serial port dumping...
#if defined(DEBUG)
  void dump() const;
#endif
};
} // namespace state
#endif