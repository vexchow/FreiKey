#include "globals.h"

// Global variables in need of instantiation

#if defined(ADAFRUIT)
BLEDis dis;
BLEHidAdafruit hid;
BLEClientUart clientUart;
BLEBas battery;
uint16_t core_handle = 0xFFFF;
#endif
