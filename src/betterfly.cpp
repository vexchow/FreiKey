#include "boardio.h"
#include "dbgcfg.h"
#include "globals.h"
#include "hardware.h"
#include "helpers.h"
#include "keymap.h"
#include "keystate.h"
#include "led_states.h"
#include "scanner.h"
#include "sleepstate.h"

constexpr BoardIO Betterfly = {{1, 0, 13, 2, 10, 11, 9, 20, 3, 14, 5, 4},
                               {23, 22, 21, 17, 16, 15}};

state::hw bfState;

// This is called when the LHS connects, disconnects, and when the system is
// initialized.  The idea is that it should just wipe everything clean.
void resetTheWorld() {
  layer_pos = 0;
  layer_stack[0] = 0;
  memset(&bfState, 0, sizeof(leftSide));
  memset(keyStates, 0xff, sizeof(keyStates));

  // TOOD: hid.keyRelease();
}

void setup(){}

void loop(){}