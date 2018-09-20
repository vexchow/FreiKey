#include "sysstuff.h"

#include "dbgcfg.h"
#include "helpers.h"
#include "bit_array.h"

#if defined(DEBUG)

void dumpVal(uint32_t v, const char* header) {
  if (header)
    Serial.print(header);
  Serial.println(v);
}

void dumpHex(uint32_t v, const char* header) {
  if (header)
    Serial.print(header);
  Serial.println(v, HEX);
}

void dumpHex(uint16_t v, const char* header) {
  dumpHex(static_cast<unsigned long>(v), header);
}

void dumpHex(uint8_t v, const char* header) {
  dumpHex(static_cast<unsigned long>(v), header);
}

void dumpHex(bool v, const char* header) {
  dumpHex(static_cast<unsigned long>(v), header);
}

const char* zeros = "00000000";
void dumpHex(uint64_t v, const char* header) {
  if (header)
    Serial.print(header);
  Serial.print(static_cast<unsigned long>(v >> 32), HEX);
  Serial.print("|");
  // Zero-pad to the left
  uint32_t lo = static_cast<unsigned long>(v);
  Serial.print(&zeros[(flsl(lo) + 3) >> 2]);
  if (lo) {
    Serial.println(lo, HEX);
  }
}

template <int T>
void dumpHex(const bit_array<T>& m, const char* header) {
  if (header)
    Serial.print(header);
  m.dumpHex();
}
template <int T>
void bit_array<T>::dumpHex() const {
  for (uint8_t i : this->value)
    ::dumpHex(i);
}
#endif
