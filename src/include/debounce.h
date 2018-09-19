#if !defined(DEBOUNCE_H)
#define DEBOUNCE_H
#include "bit_array.h"
#include "boardio.h"

BoardIO::bits debounce(const BoardIO::bits& cur_switches, uint32_t now);

#endif
