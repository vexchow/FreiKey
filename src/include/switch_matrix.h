#if !defined(SWITCH_MATRIX_H)
#define SWITCH_MATRIX_H

#include "helpers.h"
#include "mybluefruit.h"
#include <initializer_list>

template <int T>
struct switch_matrix {
  constexpr static uint32_t num_bits = T;
  constexpr static uint32_t num_bytes = (T + 7) / 8;
  uint8_t value[num_bytes];
  switch_matrix() {
    memset(&value[0], 0, sizeof(value));
  }
  switch_matrix(std::initializer_list<uint8_t> init) {
    int num = 0;
    for(uint8_t a : init) {
      if (num >= num_bytes) {
        return;
      }
      value[num++] = a;
    }
  }
  switch_matrix(bool b) {
    memset(&value[0], b ? 0xFF : 0, sizeof(value));
  }
  switch_matrix(const switch_matrix& m) {
    memcpy(&value[0], &m.value[0], sizeof(value));
  }
  bool operator==(const switch_matrix& m) const {
    return !memcmp(&value[0], &m.value[0], sizeof(value));
  }
  switch_matrix<T> delta(const switch_matrix& compare) const {
    switch_matrix<T> v{};
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