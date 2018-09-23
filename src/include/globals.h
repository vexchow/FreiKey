#if !defined(GLOBALS_H)
#define GLOBALS_H

#include "sysstuff.h"

#include "hardware.h"

#if defined(ADAFRUIT)
extern BLEDis dis;
extern BLEHidAdafruit hid;
extern BLEClientUart clientUart;
extern BLEBas battery;
extern state::hw leftSide;
extern state::hw rightSide;

extern uint16_t core_handle;
#endif

#endif
