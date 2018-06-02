#if !defined(DBGCFG_H)
#define DBGCFG_H

// This is just a couple of dumpers that show stuff on the AdaFruit serial port,
// and a couple of macros to do stuff when the system is build with DEBUG=1 (or
// 2). Set the DEBUG flag in the Makefile :)

#if DEBUG
#define DBG(a) a

void dumpVal(unsigned long v, const char* header = nullptr);
void dumpHex(unsigned long v, const char* header = nullptr);

#if DEBUG == 2
#define DBG2(a) a
#else
#define DBG2(a)
#endif
#else
#define DBG(a)
#define DBG2(a)
#endif

#if defined(__VSCODE_ONLY_)
// Stuff to make VSCode IntelliSense happy
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
extern void digitalWrite(uint32_t dwPin, uint32_t dwVal);
extern int digitalRead(uint32_t ulPin);
extern uint32_t millis(void);
extern void waitForEvent(void);
extern uint32_t analogRead(uint32_t ulPin);
extern void pinMode(uint32_t dwPin, uint32_t dwMode);
extern struct ble_gap_evt_adv_report_t;
#endif

#endif
