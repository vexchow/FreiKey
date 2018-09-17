#if !defined(SCANNER_H)
#define SCANNER_H

#include <vector>

#include "boardio.h"
#include "hardware.h"
#include "switch_matrix.h"

std::vector<scancode_t> GetScanCodesForSwitchStates(
    const switch_matrix<BoardIO::matrix_size>& beforeLeft,
    const switch_matrix<BoardIO::matrix_size>& afterLeft,
    const switch_matrix<BoardIO::matrix_size>& beforeRight,
    const switch_matrix<BoardIO::matrix_size>& afterRight);
void PerformActionsForScanCodes(const std::vector<scancode_t>& scanCodes);

#endif
