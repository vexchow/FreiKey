#include "sysstuff.h"

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
#include "usb_keyboard.h"

constexpr BoardIO Betterfly = {{1, 0, 13, 2, 10, 11, 9, 20, 3, 14, 5, 4},
                               {23, 22, 21, 17, 16, 15}};

state::hw bfState;

// This is called when the LHS connects, disconnects, and when the system is
// initialized.  The idea is that it should just wipe everything clean.
void resetTheWorld() {
  layer_pos = 0;
  layer_stack[0] = 0;
  memset(&bfState, 0, sizeof(bfState));
  memset(keyStates, 0xff, sizeof(keyStates));

  // TOOD: hid.keyRelease();
}

extern "C" void setup() {
  DBG(Serial.begin(115200));
  DBG(Serial.println("SETUP!"));
  Betterfly.Configure();
  resetTheWorld();
}

extern "C" void loop() {
  uint32_t now = millis();

  // Get the hardware state for the two sides...
  state::hw down{now, bfState, Betterfly};

  // For sleeping, look at both sides of the keyboard
  if (sleepState.CheckForSleeping(down.switches.any(), now, Betterfly)) {
    // I'm assuming this saves power. If it doesn't, there's no point...
    delay(250);
    return;
  }

  // Get the before & after of each side into a 64 bit value
  BoardIO::bits before = bfState.switches;
  BoardIO::bits after = down.switches;

  // Pseudo-code for what I'm looking to clean up:
  BoardIO::bits delta = before.delta(after);
  bool keysChanged = delta.any();

  while (delta.any()) {
    scancode_t sc;
    bool pressed;
    sc = getNextScanCode(delta, after, pressed);
    preprocessScanCode(sc, pressed, now);
  }

  if (keysChanged) {
    usb_report r = getUSBData(now);
#if defined(DEBUG) && DEBUG > 1
    Serial.print("mods=");
    Serial.print(r.mods, HEX);
    Serial.print(" repsize=");
    Serial.print(r.repsize);
    for (int i = 0; i < r.repsize; i++) {
      Serial.print(" ");
      Serial.print(r.report[i], HEX);
    }
    dumpHex(r.consumer, "Consumer:");
#endif

    // Update the hardware previous state
    bfState = down;

    // Handle the consumer stuff:
    if (r.consumer) {
      if (r.consumer > 0) {
        DBG2(dumpHex(r.consumer, "Consumer key press: "));
        // Keyboard.press(r.consumer);
      } else {
        DBG2(dumpHex(r.consumer, "Consumer key release: "));
        // Keyboard.release(-r.consumer);
      }
    }
    if (r.repsize) {
      // memcpy(keyboard_keys, r.report, sizeof(keyboard_keys));
      // keyboard_modifier_keys = r.mods;
      // usb_keyboard_send();
      DBG2(Serial.println("============================="));
      DBG2(down.dump());
    }
  }
}
