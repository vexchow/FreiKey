#if !defined(SCANNER_H)
#define SCANNER_H

#include <vector>

#include "hardware.h"
#include "switch_matrix.h"
std::vector<scancode_t> GetScanCodesForSwitchStates(
    const switch_matrix& beforeLeft,
    const switch_matrix& afterLeft,
    const switch_matrix& beforeRight,
    const switch_matrix& afterRight);
void PerformActionsForScanCodes(const std::vector<scancode_t>& scanCodes);

#endif
