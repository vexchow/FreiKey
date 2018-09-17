#if !defined(DEBOUNCE_H)
#define DEBOUNCE_H
#include "boardio.h"
#include "switch_matrix.h"

switch_matrix<BoardIO::matrix_size> debounce(
    const switch_matrix<BoardIO::matrix_size>& cur_switches, uint32_t now);

#endif