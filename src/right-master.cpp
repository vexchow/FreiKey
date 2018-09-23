#include "sysstuff.h"

#include "boardio.h"
#include "callbacks.h"
#include "dbgcfg.h"
#include "globals.h"
#include "hardware.h"
#include "helpers.h"
#include "keystate.h"
#include "led_states.h"
#include "scanner.h"
#include "sleepstate.h"
#include "status_dump.h"

// I'm going to update this to keep track of additional state.
// Each key 'previously' pressed should have a 'time last pressed'
// Maybe keep track of whether other keys have been pressed since?
//  Perhaps have a flag for something that's a "Normal" key?
// The core idea:
//  I want to be able to trigger something off of both the "key pressed"
//  as well as the "key released". In addition, I want to do things that
//  'cancel out' the key state if something specific occurs

// Kind of things I want to do:
// #1: Better, intentional debouncing
// #2: Key-up triggering
// #3: Cancellation
// #4: Key-state back & forth (caps as a real cmd/ctrl, instead of a layer
// shift)

// These numbers correspond to the *port pin* numbers in the nRF52 documentation
// not the physical pin numbers...
constexpr BoardIO RightBoard = {
    {29, 16, 15, 7, 27, 11, 30}, {13, 4, 2, 3, 5, 12}, 28};

state::hw leftSide{};
state::hw rightSide{};

// The are the top left & right keys, plus the lowest 'outer' key
const BoardIO::bits status_clear_bonds_left{
    {1, 2, 3, 4, 5}}; // 0x10000000042ULL;
const BoardIO::bits status_clear_bonds_right{
    {1, 2, 3, 4, 5}}; // 0x1000000021ULL;

// This is called when the LHS connects, disconnects, and when the system is
// initialized.  The idea is that it should just wipe everything clean.
void resetTheWorld() {
  layer_pos = 0;
  layer_stack[0] = 0;
  memset(&leftSide, 0, sizeof(leftSide));
  memset(&rightSide, 0, sizeof(rightSide));
  memset(keyStates, 0xff, sizeof(keyStates));

  hid.keyRelease();
}

const state::led* curState = nullptr;
uint32_t stateTime = 0;

// Check to see if we should update the battery level and if so, do so
void updateBatteryLevel(const state::hw& downLeft, const state::hw& downRight) {
  if (downRight.battery_level != rightSide.battery_level ||
      downLeft.battery_level != leftSide.battery_level) {
    // We only get the battery level from the left side once you hit a key, so
    // only report it if we have something to actually report
    if (downLeft.battery_level) {
      battery.notify((downRight.battery_level + downLeft.battery_level) / 2);
      DBG(dumpVal((downRight.battery_level + downLeft.battery_level) / 2,
                  "battery avg: "));
    } else {
      DBG(dumpVal(downRight.battery_level, "right only battery: "));
      battery.notify(downRight.battery_level);
    }
    rightSide.battery_level = downRight.battery_level;
    leftSide.battery_level = downLeft.battery_level;
  }
}

SleepState sleepState = {0, false, true};

void loop() {
  uint32_t now = millis();

  // Get the hardware state for the two sides...
  state::hw downRight{now, rightSide, RightBoard};
  state::hw downLeft{clientUart, leftSide};

  // For sleeping, look at both sides of the keyboard
  bool down = downRight.switches.any() || downLeft.switches.any();
  if (sleepState.CheckForSleeping(down, now, RightBoard)) {
    // I'm assuming this saves power. If it doesn't, there's no point...
    delay(250);
    waitForEvent();
    return;
  }

  // Update the combined battery level
  updateBatteryLevel(downLeft, downRight);

  // Get the before & after of each side into a 64 bit value
  BoardIO::bits beforeLeft = leftSide.switches;
  BoardIO::bits afterLeft = downLeft.switches;
  BoardIO::bits beforeRight = rightSide.switches;
  BoardIO::bits afterRight = downRight.switches;

  // Pseudo-code for what I'm looking to clean up:
#if 0
  std::vector<scancode_t> scanCodes = GetScanCodesForSwitchStates(
      beforeLeft, afterLeft, beforeRight, afterRight);
  PerformActionsForScanCodes(scanCodes);
#endif
  BoardIO::bits deltaLeft = beforeLeft.delta(afterLeft);
  BoardIO::bits deltaRight = beforeRight.delta(afterRight);
  bool keysChanged = deltaLeft.any() || deltaRight.any();
  if (deltaRight.any() && !curState) {
    // if we're not already in a state, check to see if we're transitioning
    // into one
    curState = state::led::get(downRight, layer_pos + 1);
    if (curState) {
      stateTime = now;
    }
  }

  if (curState) {
    // We're in some random LED display state. Do something...
    if (now - curState->time < stateTime) {
      RightBoard.setLED(curState->get_led_value(downRight, now - stateTime));
    } else {
      RightBoard.setLED(0);
      curState = nullptr;
    }
  }

  while (deltaLeft.any() || deltaRight.any()) {
    scancode_t sc;
    bool pressed;
    if (deltaLeft.any()) {
      sc = getNextScanCode(deltaLeft, afterLeft, pressed);
    } else {
      // Add offset to the right scan code...
      sc = getNextScanCode(deltaRight, afterRight, pressed) +
           BoardIO::matrix_size;
    }
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
    rightSide = downRight;
    leftSide = downLeft;

    // Handle the consumer stuff:
    if (r.consumer) {
      if (r.consumer > 0) {
        DBG2(dumpHex(r.consumer, "Consumer key press: "));
        hid.consumerKeyPress(r.consumer);
      } else {
        DBG2(dumpHex(r.consumer, "Consumer key release: "));
        hid.consumerKeyRelease();
      }
    }

#if defined(STATUS_DUMP)
    // If we do a status dump, don't pass the keys pressed on to the computer...
    if (!status_dump_check(rightSide, leftSide))
#endif
      hid.keyboardReport(r.mods, r.report);
    DBG2(Serial.println("============================="));
    DBG2(Serial.print("Left side "));
    DBG2(downLeft.dump());
    DBG2(Serial.print("Right side "));
    DBG2(downRight.dump());

    if (rightSide.switches == status_clear_bonds_right &&
        leftSide.switches == status_clear_bonds_left) {
      DBG(Serial.println("CLEARING BLUETOOTH BONDS!"));
      Bluefruit.clearBonds();
    }
  }

  waitForEvent(); // Request CPU enter low-power mode until an event occurs
}

// In Arduino world the 'setup' function is called to initialize the device.
// The 'loop' function is called over & over again, after setup completes.
void setup() {
  DBG(Serial.begin(115200));
  RightBoard.Configure();
  resetTheWorld();

  // Central and peripheral
  Bluefruit.begin(true, true);
  // Bluefruit.clearBonds();
  Bluefruit.autoConnLed(true);

  battery.begin();

  // I'm assuming that by dropping this power down, I'll save some battery life.
  // I should experiment to see how low I can get it and still communicate with
  // both my Mac and my PC reliably. They're each within a meter of the
  // keyboard... Acceptable values: -40, -30, -20, -16, -12, -8, -4, 0, 4
  Bluefruit.setTxPower(4);
  Bluefruit.setName(BT_NAME);

  Bluefruit.Central.setConnectCallback(callback::cent_connect);
  Bluefruit.Central.setDisconnectCallback(callback::cent_disconnect);

  dis.setManufacturer(MANUFACTURER);
  dis.setModel(MODEL);
  dis.setHardwareRev(HW_REV);
  dis.setSoftwareRev(__DATE__);
  dis.begin();

  clientUart.begin();
  Bluefruit.setConnectCallback(callback::core_connect);
  Bluefruit.setDisconnectCallback(callback::core_disconnect);
  // clientUart.setRxCallback(cent_bleuart_rx_callback);

  /* Start Central Scanning
   * - Enable auto scan if disconnected
   * - Interval = 100 ms, window = 80 ms
   * - Filter only accept bleuart service
   * - Don't use active scan
   * - Start(timeout) with timeout = 0 will scan forever (until connected)
   */
  Bluefruit.Scanner.setRxCallback(callback::scan);
  Bluefruit.Scanner.restartOnDisconnect(true);
  Bluefruit.Scanner.setInterval(160, 80); // in unit of 0.625 ms
  Bluefruit.Scanner.filterUuid(BLEUART_UUID_SERVICE);
  Bluefruit.Scanner.useActiveScan(false);
  // I should probably stop advertising after a while if that's possible. I have
  // switches now, so if I need it to advertise, I can just punch the power.
  Bluefruit.Scanner.start(0); // 0 = Don't stop scanning after n seconds

  hid.begin();
  // delay(5000);
  // Bluefruit.printInfo();

  // This gets Advertising going...
  Bluefruit.Advertising.addFlags(BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE);
  Bluefruit.Advertising.addTxPower();
  Bluefruit.Advertising.addAppearance(BLE_APPEARANCE_HID_KEYBOARD);
  Bluefruit.Advertising.addService(hid);

  Bluefruit.ScanResponse.addService(battery); // This doesn't seem to work :(

  Bluefruit.Advertising.addName();
  Bluefruit.Advertising.restartOnDisconnect(true);
  Bluefruit.Advertising.setInterval(32, 244); // in unit of 0.625 ms
  Bluefruit.Advertising.setFastTimeout(30); // number of seconds in fast mode
  Bluefruit.Advertising.start(0); // 0 = Don't stop advertising after n seconds
}
