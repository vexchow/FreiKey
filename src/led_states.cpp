#include "led_states.h"
/*
struct led_state {
  uint64_t left_state;
  uint64_t right_state;
  uint32_t (*get_led_value)(const hwstate &switches, uint32_t time_offset);
  uint32_t time;
};

const led_state *getState(const hwstate &switches, bool left = false);
*/

uint32_t batteryFlasher(const hwstate& sw, uint32_t time_offset) {
  if (sw.battery_level > 15) {
    return abs(16 - (time_offset / 4) & 31);
  } else {
    return 10;
  }
}

// TODO: make some blinky states to indicate what mode the keyboard is in
uint32_t macMode(const hwstate& sw, uint32_t time_offset) {
  return 0;
}

constexpr led_state key_states[] = {
    // These are the lower & outer 3 keys to get the battery status
    {0x10408000000ULL, 0x1010200000ULL, batteryFlasher, 1000}};

const led_state* getState(const hwstate& sw, uint8_t layer) {
  for (auto& st : key_states) {
    if (sw.switches == ((!layer) ? st.left_state : st.right_state)) {
      return &st;
    }
  }
  return nullptr;
}