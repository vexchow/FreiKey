#include <vector>

#include "dbgcfg.h"
#include "keymap.h"
#include "keystate.h"
#include "scanner.h"

// This stuff is WIP, and I'm not doing it yet. Maybe some day...
std::vector<scancode_t> GetScanCodesForSwitchStates(BoardIO::bits beforeLeft,
                                                    BoardIO::bits afterLeft,
                                                    BoardIO::bits beforeRight,
                                                    BoardIO::bits afterRight) {
  return std::vector<scancode_t>{};
}
void PerformActionsForScanCodes(const std::vector<scancode_t>& scanCodes) {
  return;
}

// Declarations

keystate keyStates[16];
layer_t layer_stack[layer_max + 1];
layer_t layer_pos = 0;

#if defined(DEBUG)
void dumpScanCode(uint8_t sc, bool pressed) {
  Serial.print("Scan Code ");
  Serial.print(sc, HEX);
  Serial.print(" was ");
  Serial.println(pressed ? "pressed" : "released");
}
void dumpLayers() {
  Serial.print("Layer stack: ");
  for (int i = 0; i <= layer_pos; i++) {
    Serial.print(layer_stack[i]);
    Serial.print(" ");
  }
  Serial.println("");
}
#endif

// Look for a slot that is either already in use for this scan code, or vacant.
// If we don't have a vacant slot, return the oldest, but still in use, slot,
// but only for key-up states, as we're probably through with them anyway.
struct keystate* findStateSlot(scancode_t scanCode) {
  keystate *vacant = nullptr, *reap = nullptr;
  for (auto& s : keyStates) {
    // If we have the same scan code, huzzah!
    if (s.scanCode == scanCode) {
      return &s;
    }
    // If we found a vacancy, potentially use it. We have to keep looking to see
    // if we have the same scan code, though.
    if (s.scanCode == null_scan_code) {
      vacant = &s;
    } else if (!s.down) {
      if (!reap) {
        reap = &s;
      } else if (s.lastChange < reap->lastChange) {
        // Idle longer than the other reapable candidate; choose
        // the eldest of them
        reap = &s;
      }
    }
  }
  if (vacant) {
    reap = vacant;
  }
  return reap;
}

// Find the first specified action in the layer stack
action_t resolveActionForScanCodeOnActiveLayer(uint8_t scanCode) {
  layer_t s = layer_pos;
  while (s > 0 && keymap[layer_stack[s]][scanCode] == ___) {
    --s;
  }
#if defined(DEBUG)
  Serial.println("Resolving action to this:");
  dumpHex(keymap[layer_stack[s]][scanCode]);
#endif
  return keymap[layer_stack[s]][scanCode];
}

// Given a delta mask, get the scan code, update the delta mask and set pressed
// while we're at it.
scancode_t getNextScanCode(BoardIO::bits& delta,
                           BoardIO::bits& curState,
                           bool& pressed) {
  scancode_t sc = delta.pull_a_bit();
  pressed = curState.get_bit(sc);
  return sc;
}

void layer_push(layer_t layer) {
  DBG(dumpVal(layer, "Push "));
  if (layer_pos < layer_max)
    layer_stack[++layer_pos] = layer;
  DBG(dumpLayers());
}

void layer_pop(layer_t layer) {
  DBG(dumpVal(layer, "Pop "));
  if (layer_pos > 0)
    --layer_pos;
  DBG(dumpLayers());
}

void layer_toggle(layer_t layer) {
  // Toggling a layer: If it exists *anywhere* in the layer stack, turn it
  // off (and fold the layer stack down) If it's *not* in the layer stack,
  // add it.
  for (layer_t l = layer_pos; l != 0; l--) {
    if (layer_stack[l] == layer) {
      DBG(dumpVal(layer, "Turning off layer "));
      DBG(dumpVal(l, "at location "));
      if (layer_pos != l) {
        DBG(dumpVal(layer_pos - l, "Shifting by "));
        memmove(&layer_stack[l], &layer_stack[l + 1], layer_pos - l);
      }
      layer_pos--;
      DBG(dumpLayers());
      return;
    }
  }
  DBG(Serial.print("(For Toggle) "));
  layer_push(layer);
}

void layer_switch(layer_t layer) {
  DBG(dumpVal(layer_stack[layer_pos], "Switching layer "));
  DBG(dumpVal(layer, "to layer "));
  layer_stack[layer_pos] = layer;
  DBG(dumpLayers());
}

void preprocessScanCode(scancode_t sc, bool pressed, uint32_t now) {
  DBG2(dumpScanCode(sc, pressed));
  // Get a state slot for this scan code
  keystate* state = findStateSlot(sc);
  DBG(state->dump());
  if (!state) {
    // If this is a keydown and we don't have an available state slot just
    // ignore it. If we chose to toss out older keydowns instead, things could
    // get pretty weird. If this is a keyup, and we still don't have a state
    // slot, that's a little bonkers, but there's not much we can do about it.
    return;
  }
  // State update returns a layer action to perform...
  switch (state->update(sc, pressed, now)) {
    case kPushLayer:
      layer_push(state->get_layer());
      break;
    case kPopLayer:
      layer_pop(state->get_layer());
      break;
    case kToggleLayer:
      layer_toggle(state->get_layer());
      break;
    case kSwitchLayer:
      layer_switch(state->get_layer());
      break;
  }
}

usb_report getUSBData(uint32_t now) {
  usb_report res;
  memset(&res, 0, sizeof(res));

  for (auto& state : keyStates) {
    if (state.scanCode == null_scan_code)
      continue;
    DBG(state.dump());
    if ((state.action & kConsumer) == kConsumer) {
      DBG(Serial.println("Got a consumer action"));
      if (res.consumer) {
        DBG(Serial.println("Trying to press multiple consumer keys at once."));
      }
      // For a consumer control button, there are no modifiers, it's
      // just a simple call. So just call it directly:
      res.consumer = (state.action & 0xFF) * (state.down ? 1 : -1);
      if (!state.down) {
        // We have to clear this thing out when we're done, because we take
        // action on the key release as well. We don't do this for the normal
        // keyboardReport.
        state.scanCode = null_scan_code;
      }
    } else if (state.down) {
      uint8_t key;
      switch (state.action & kMask) {
        case kModifier:
          res.mods |= state.action & 0xff;
          break;
        case kToggleMod:
          res.mods ^= state.action & 0xff;
          break;
        case kTapHold:
          if (now - state.lastChange > 200) {
            // Holding
            res.mods |= (state.action >> 16) & 0xff;
          } else {
            // Tapsg
            key = state.action & 0xff;
            if (key != 0 && res.repsize < 6) {
              res.report[res.repsize++] = key;
            }
          }
          break;
        case kKeyAndMod:
          res.mods |= (state.action >> 16) & 0xff;
          key = state.action & 0xff;
          if (key != 0 && res.repsize < 6) {
            res.report[res.repsize++] = key;
          }
          break;
        case kKeyPress:
          key = state.action & 0xff;
          if (key != 0 && res.repsize < 6) {
            res.report[res.repsize++] = key;
          }
          break;
      }
    }
  }
  return res;
}
