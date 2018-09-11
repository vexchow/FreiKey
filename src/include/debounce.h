#if !defined(DEBOUNCE_H)
#define DEBOUNCE_H
#include "switch_matrix.h"

switch_matrix debounce(const switch_matrix& cur_switches, uint32_t now);

#endif