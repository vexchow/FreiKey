#if !defined(SWITCH_MATRIX_H)
#define SWITCH_MATRIX_H

#include "helpers.h"
#include "mybluefruit.h"

struct switch_matrix {
  static const uint32_t num_bits = 64;
  uint64_t value;
  switch_matrix() : value(0) {}
  constexpr switch_matrix(uint64_t v) : value(v) {}
  switch_matrix(bool b) : value(b ? static_cast<uint64_t>(-1ULL) : 0) {}
  switch_matrix(const switch_matrix& m) : value(m.value) {}
  bool operator==(const switch_matrix& m) const {
    return m.value == value;
  }
  switch_matrix delta(const switch_matrix& compare) const {
    uint64_t v = value ^ compare.value;
    return switch_matrix{v};
  }
  bool any() const {
    return !!value;
  }
  uint8_t pull_a_bit() {
    uint8_t bit_num = flsl(value);
    uint64_t mask = static_cast<uint64_t>(1) << bit_num;
    value ^= mask;
    return bit_num;
  }
  void flip_bit(uint8_t bitnum) {
    value ^= 1ULL << bitnum;
  }
  void set_bit(uint8_t bitnum) {
    value |= 1ULL << bitnum;
  }
  bool get_bit(uint8_t bitnum) const {
    return !!value & (1ULL << bitnum);
  }
#if defined(DEBUG)
  void dumpHex() const;
#endif
};

#endif