# Grabbed some stuff from Teensyduino to modify the stuff I slapped together
# for Adafruit. Initially, I'm only getting it working on MacOS, because reasons

# Some simple details
OUT=teensy

# AdaFruit library installation location/version malarkey
ifeq ($(OS),Windows_NT)
	uname:=Windows
else
	uname:=$(shell uname -s)
endif

SSD1306_ROOT=../../Adafruit_SSD1306
GFX_ROOT=../../Adafruit-GFX-Library

ifeq ($(uname), Windows)
PORT=COM7
RMFILES=del /q
RMDIR=rmdir /s /q
DIV=$(strip \ )
else
ARDUINOROOT=/Applications/Arduino.app/Contents/Java
DPORT=/dev/cu.SLAB_USBtoUART
RMFILES=rm
RMDIR=rm -rf
DIV=/
endif

MCU=MK20DX256
MKU_LD=mk20dx256.ld

TOOLS=${ARDUINOROOT}/hardware/tools
AVR=${ARDUINOROOT}/hardware/teensy/avr

# Tools (probably don't need to change these at all)
CC=${TOOLS}/arm/bin/arm-none-eabi-gcc
CPP=${TOOLS}/arm/bin/arm-none-eabi-g++
OBJCOPY=${TOOLS}/arm/bin/arm-none-eabi-objcopy
AR=${TOOLS}/arm/bin/arm-none-eabi-ar

# Flags for compilation
# First, DEBUG and STATUS_DUMP configuration flags (then everything else)
# F_CPU = CPU Frequency. 96MHz! Boo yah!!!!
DEFINES=-DDEBUG \
-DSTATUS_DUMP \
-DF_CPU=96000000 \
-DARDUINO=10805 \
-DUSB_SERIAL \
-DLAYOUT_US_ENGLISH \
-DUSING_MAKEFILE \
-D__$(MCU)__ \
-DARDUINO_BSP_VERSION=\"${VER}\" \
-DTEENSYDUINO=143 \
-DTEENSY
TARGET=-mcpu=cortex-m4 -mthumb
CODEGEN=-ffunction-sections -fdata-sections
FLAGS=-g -Wall -u _printf_float -MMD
CPPLANG=-std=gnu++14 -w -x c++ -fno-rtti -fno-exceptions -fno-threadsafe-statics -felide-constructors
CLANG=-std=gnu14
SLANG=-x assembler-with-cpp
OPT=-Os

SHARED_SRC = dbgcfg.cpp hardware.cpp led_states.cpp boardio.cpp debounce.cpp \
	sleepstate.cpp
BETTERFLY_SRC = status_dump.cpp globals.cpp betterfly.cpp scanner.cpp
DISP_SRC = display.cpp disp_uart.cpp
USER_SRC = ${SHARED_SRC} ${BETTERFLY_SRC} ${DISP_SRC}

INCLUDES=-Iinclude \
  "-I${AVR}/cores/teensy3"\
  "-I${AVR}/libraries/SPI"\
  "-I${AVR}/libraries/Wire"\
	"-I${GFX_ROOT}"\
	"-I${SSD1306_ROOT}"\


ELF_FLAGS = ${OPT} ${TARGET} -save-temps \
-Wl,--defsym=__rtc_localtime=0 \
-Wl,--cref \
-Wl,--check-sections \
-Wl,--gc-sections \
-Wl,--unresolved-symbols=report-all \
-Wl,--warn-common \
-Wl,--warn-section-align \
--specs=nano.specs

CPPFLAGS=${TARGET} ${FLAGS} ${CODEGEN} ${CPPLANG} ${DEFINES} ${OPT} ${INCLUDES}
CFLAGS=${TARGET} ${FLAGS} ${CODEGEN} ${CLANG} ${DEFINES} ${OPT} ${INCLUDES}
SFLAGS=${TARGET} ${FLAGS} ${CODEGEN} ${SLANG} ${DEFINES} ${OPT} ${INCLUDES}

GFX_SRCS = \
	${SSD1306_ROOT}/Adafruit_SSD1306.cpp \
	${GFX_ROOT}/Adafruit_GFX.cpp \
	${GFX_ROOT}/Adafruit_SPITFT.cpp

GFX_OBJS = \
	$(addprefix ${OUT}/, $(patsubst %.cpp, %.cpp.o, $(notdir ${GFX_SRCS})))

SHARED_OBJS = $(addprefix ${OUT}/, $(patsubst %.cpp, %.cpp.o, ${SHARED_SRC}))
RIGHT_OBJS = $(addprefix ${OUT}/, $(patsubst %.cpp, %.cpp.o, ${RIGHT_SRC}))
LEFT_OBJS = $(addprefix ${OUT}/, $(patsubst %.cpp, %.cpp.o, ${LEFT_SRC}))
DISP_OBJS = $(addprefix ${OUT}/, $(patsubst %.cpp, %.cpp.o, ${DISP_SRC}))
USER_OBJS =  $(addprefix ${OUT}/, $(patsubst %.cpp, %.cpp.o, ${USER_SRC}))


.PHONY: clean allclean depclean betterfly flash

all: ${OUT} betterfly

DEPS = $(USER_OBJS:.o=.d) $(GFX_OBJS:.o=.d)

-include ${DEPS}

flash: ${OUT}/betterfly.hex
	${NRFUTIL} --verbose dfu serial -pkg $< -p ${DPORT} -b 115200

betterfly: ${OUT}/betterfly.hex

# Make us rebuild my own source if I change the makefile
$(USER_OBJS) : Makefile

${OUT}/%.hex : ${OUT}/%.elf
	${OBJCOPY} -O ihex -R .eeprom $< $@

${OUT}/betterfly.elf : ${USER_OBJS} ${GFX_OBJS}
	${CC} ${ELF_FLAGS} "-Wl,-Map,$@.map" -o $@ $^ \
	-Wl,--start-group -lm ${ALL_LIBS} -Wl,--end-group

${OUT}:
	@-mkdir ${OUT}

allclean:
	@-${RMDIR} ${OUT}

clean:
	@-${RMFILES} $(subst /,${DIV},${USER_OBJS} ${OUT}$*.elf ${OUT}/*.zip ${OUT}/*.hex ${OUT}${DIV}*.map)

depclean:
	@-${RMFILES} ${DEPS}

# And now all the custom subdir rules :/

${OUT}/%.cpp.o: %.cpp
	${CPP} -c ${CPPFLAGS} -o $@ $<

${OUT}/%.cpp.o: ${GFX_ROOT}/%.cpp
	${CPP} -c ${CPPFLAGS} -o $@ $<

${OUT}/%.cpp.o: ${SSD1306_ROOT}/%.cpp
	${CPP} -c ${CPPFLAGS} -o $@ $<
