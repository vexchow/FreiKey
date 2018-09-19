#if !defined(SCANNER_H)
#define SCANNER_H

#include <vector>

#include "bit_array.h"
#include "boardio.h"
#include "hardware.h"

std::vector<scancode_t> GetScanCodesForSwitchStates(
    const BoardIO::bits& beforeLeft,
    const BoardIO::bits& afterLeft,
    const BoardIO::bits& beforeRight,
    const BoardIO::bits& afterRight);
void PerformActionsForScanCodes(const std::vector<scancode_t>& scanCodes);

#endif
