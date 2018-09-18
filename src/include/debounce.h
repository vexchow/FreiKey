#if !defined(DEBOUNCE_H)
#define DEBOUNCE_H
#include "boardio.h"
#include "bit_array.h"

bit_array<BoardIO::matrix_size> debounce(
    const bit_array<BoardIO::matrix_size>& cur_switches, uint32_t now);

#endif
