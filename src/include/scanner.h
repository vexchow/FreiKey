#if !defined(SCANNER_H)
#define SCANNER_H

#include "sysstuff.h"
#include <vector>

#include "bit_array.h"
#include "boardio.h"
#include "dbgcfg.h"
#include "hardware.h"
#include "keyhelpers.h"
#include "keystate.h"

// variable declarations

extern keystate keyStates[16];
constexpr layer_t layer_max = 7;
extern layer_t layer_stack[layer_max + 1];
extern layer_t layer_pos;

// Structs
struct usb_report {
  uint8_t report[6];
  uint8_t repsize;
  uint8_t mods;
  uint16_t consumer; // negative = release
};

// Functions
void layer_switch(layer_t layer);
void layer_toggle(layer_t layer);
void layer_pop(layer_t layer);
void layer_push(layer_t layer);
scancode_t getNextScanCode(BoardIO::bits& delta,
                           BoardIO::bits& curState,
                           bool& pressed);
keystate* findStateSlot(uint8_t scanCode);
void preprocessScanCode(scancode_t sc, bool pressed, uint32_t now);
usb_report getUSBData(uint32_t now);

// Stuff I started, but haven't done anything with:
std::vector<scancode_t> GetScanCodesForSwitchStates(
    const BoardIO::bits& beforeLeft,
    const BoardIO::bits& afterLeft,
    const BoardIO::bits& beforeRight,
    const BoardIO::bits& afterRight);
void PerformActionsForScanCodes(const std::vector<scancode_t>& scanCodes);

#endif
