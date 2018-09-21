#include "sleepstate.h"
#include "boardio.h"
#include "hardware.h"

bool SleepState::CheckForSleeping(bool pressed,
                                  uint32_t time,
                                  const BoardIO& board) {
  // If we're in forced sleep mode, don't wake up
  if (!forced) {
    // First, handle sleeping states
    if (pressed) {
      // We detected a keypress!
      if (sleeping) {
#if defined(HAS_LED)
        // Turn off the LED if we were sleeping
        board.setLED(0);
#endif
        DBG(dumpVal(lastPressTime, "Exiting sleep from "));
      }
      sleeping = false;
      lastPressTime = time;
    } else if (!sleeping && (time - lastPressTime > 300000)) {
      // 5 minutes before we sleep
      // Do other stuff to get into low power mode, here!
      sleeping = true;
      DBG(dumpVal(time, "Entering sleep from "));
    }
  }
#if defined(HAS_LED)
  if (sleeping || forced) {
    // Blink the LED a little bit
    uint8_t brightness = !((time >> 9) & 3);
    board.setLED(brightness);
  }
#endif
  return sleeping;
}

void SleepState::BeginForcedSleepMode() {
  DBG(dumpVal(lastPressTime, "Forced sleep at "));
  forced = true;
}

void SleepState::EndForcedSleepMode() {
  DBG(dumpVal(lastPressTime, "Forced sleep stopped at "));
  forced = false;
  sleeping = true;
}
