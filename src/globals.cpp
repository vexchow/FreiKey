#include "globals.h"
#include "sleepstate.h"

// Global variables in need of instantiation

#if !defined(TEENSY)
BLEDis dis;
BLEHidAdafruit hid;
BLEClientUart clientUart;
BLEBas battery;
uint16_t core_handle = 0xFFFF;
#endif

SleepState sleepState = {0, false, true};
