#if !defined(LED_STATES_H)
#define LED_STATES_H

#include "hardware.h"
#include "switch_matrix.h"

namespace state {

struct led {
  switch_matrix left_state;
  switch_matrix right_state;
  uint32_t (*get_led_value)(const state::hw& switches, uint32_t time_offset);
  uint32_t time;
  static const led* get(const state::hw& switches, uint8_t layer = 0);
};

} // namespace state
#endif
