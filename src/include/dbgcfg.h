#if !defined(DBGCFG_H)
#define DBGCFG_H

#if !defined(ADAFRUIT) && !defined(TEENSY)
#error Sorry: Unsupported platform
#endif

#include <cstdint>

// This is just a couple of dumpers that show stuff on the AdaFruit serial port,
// and a couple of macros to do stuff when the system is build with DEBUG=1 (or
// 2). Set the DEBUG flag in the Makefile :)
#if defined(DEBUG)
template<int T>
struct bit_array;
void dumpVal(unsigned long v, const char* header = nullptr);
void dumpHex(uint8_t v, const char* header = nullptr);
void dumpHex(uint16_t v, const char* header = nullptr);
void dumpHex(uint32_t v, const char* header = nullptr);
void dumpHex(uint64_t v, const char* header = nullptr);
void dumpHex(bool b, const char* header = nullptr);
void dumpStr(const char *str);
template <int T>
void dumpHex(const bit_array<T>& m, const char* header = nullptr) {
  if (header)
    dumpStr(header);
  m.dumpHex("");
}

#define DBG(a) a
#endif

#if defined(DEBUG) && DEBUG == 2
#define DBG2(a) a
#else
#define DBG2(a)
#endif

#if !defined(DEBUG)
#define DBG(a)
#define DBG2(a)
#endif

#if defined(__VSCODE_ONLY_)
// Stuff to make VSCode IntelliSense happy (happier, since it's still kinda sad...)
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
extern void digitalWrite(uint8_t dwPin, uint8_t dwVal);
extern uint8_t digitalRead(uint8_t ulPin);
extern void delay(uint32_t);
extern void waitForEvent(void);
extern int analogRead(uint8_t ulPin);
extern void analogWrite(uint8_t ulPin, int value);
extern void pinMode(uint8_t dwPin, uint8_t dwMode);
struct ble_gap_evt_adv_report_t;
#endif

#endif
