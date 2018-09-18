#if !defined(LED_STATES_H)
#define LED_STATES_H

#include "hardware.h"
#include "bit_array.h"
#include "boardio.h"
namespace state {

struct led {
  bit_array<BoardIO::matrix_size> left_state;
  bit_array<BoardIO::matrix_size> right_state;
  uint32_t (*get_led_value)(const state::hw& switches, uint32_t time_offset);
  uint32_t time;
  static const led* get(const state::hw& switches, uint8_t layer = 0);
};

} // namespace state
#endif
