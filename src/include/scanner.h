#if !defined(SCANNER_H)
#define SCANNER_H

#include <vector>

#include "boardio.h"
#include "hardware.h"
#include "bit_array.h"

std::vector<scancode_t> GetScanCodesForSwitchStates(
    const bit_array<BoardIO::matrix_size>& beforeLeft,
    const bit_array<BoardIO::matrix_size>& afterLeft,
    const bit_array<BoardIO::matrix_size>& beforeRight,
    const bit_array<BoardIO::matrix_size>& afterRight);
void PerformActionsForScanCodes(const std::vector<scancode_t>& scanCodes);

#endif
