#if !defined(SWITCH_MATRIX_H)
#define SWITCH_MATRIX_H

#include <initializer_list>

#include "helpers.h"
#include "mybluefruit.h"

template <int T>
struct bit_array {
  constexpr static uint32_t num_bits = T;
  constexpr static uint32_t num_bytes = (T + 7) / 8;
  uint8_t value[num_bytes];
  bit_array() {
    memset(&value[0], 0, sizeof(value));
  }
  bit_array(std::initializer_list<uint8_t> init) {
    int num = 0;
    for (uint8_t a : init) {
      if (num >= num_bytes) {
        return;
      }
      value[num++] = a;
    }
  }
  bit_array(const bit_array& m) {
    memcpy(&value[0], &m.value[0], sizeof(value));
  }
  bool operator==(const bit_array& m) const {
    return !memcmp(&value[0], &m.value[0], sizeof(value));
  }
  bit_array<T> delta(const bit_array& compare) const {
    bit_array<T> v{};
    for (size_t i = 0; i < sizeof(value); i++) {
      v.value[i] = value[i] ^ compare.value[i];
    }
    return v;
  }
  bool any() const {
    for (uint8_t i : value) {
      if (i)
        return true;
    }
    return false;
  }
  uint8_t pull_a_bit() {
    for (int i = 0; i < sizeof(value); i++) {
      if (value[i]) {
        uint8_t bit_num = flsl(value[i]);
        value[i] ^= (1 << bit_num);
        return bit_num + i * 8;
      }
    }
  }
  void flip_bit(uint8_t bitnum) {
    value[bitnum >> 3] ^= 1 << (bitnum & 7);
  }
  void set_bit(uint8_t bitnum) {
    value[bitnum >> 3] |= 1 << (bitnum & 7);
  }
  bool get_bit(uint8_t bitnum) const {
    return !!(value[bitnum >> 3] & (1 << (bitnum & 7)));
  }
#if defined(DEBUG)
  void dumpHex() const;
#endif
};

#endif
