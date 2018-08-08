#if !defined(GLOBALS_H)
#define GLOBALS_H

#include "mybluefruit.h"

#include "sleepstate.h"
#include "hardware.h"

extern BLEDis dis;
extern BLEHidAdafruit hid;
extern BLEClientUart clientUart;
extern BLEClientUart displayUart;
extern BLEBas battery;

extern state::hw leftSide;
extern state::hw rightSide;

extern uint16_t core_handle;
extern SleepState sleepState;

#endif
