MCU = atmega328p
MCU_DEFINE = __AVR_ATmega328P__
F_CPU = 16000000
BAUD = 9600
TX_BUFFER_SIZE = 256
RX_BUFFER_SIZE = 256
SERIAL_COMM_TIMEOUT = 3000

SRC_DIR = src
BUILD_DIR = build
OBJ_DIR = ${BUILD_DIR}/obj
DEP_DIR = ${BUILD_DIR}/dep

SRCS = main.c uart.c util.c
OBJS := ${SRCS:%.c=${OBJ_DIR}/%.o}
DEPS = ${OBJS:.o=.d}
TARGET_INCLUDE_DIRS = include
LIBRARY_INCLUDE_DIRS = /usr/avr/include /usr/lib/gcc/avr/11.2.0/include
INCLUDE_DIRS = ${TARGET_INCLUDE_DIRS} ${LIBRARY_INCLUDE_DIRS}

CC=avr-gcc
CFLAGS= -MMD -std=c11 -Wall -g -Os -mmcu=${MCU} -nostdinc ${INCLUDE_DIRS:%=-I%} -DF_CPU=${F_CPU} -DBAUD=${BAUD} -DTX_BUFFER_SIZE=${TX_BUFFER_SIZE} -DRX_BUFFER_SIZE=${RX_BUFFER_SIZE} -DSERIAL_COMM_TIMEOUT=${SERIAL_COMM_TIMEOUT}
OBJCPY=avr-objcopy
TARGET=RW_FLASH_MICRO
PROG_METHOD=avrisp
SERIAL_PORT=/dev/ttyUSB0

.PHONY: all
all: ${BUILD_DIR}/${TARGET}
	${OBJCPY} -j .text -j .data -O ihex ${TARGET}.bin ${TARGET}.hex

${OBJ_DIR}/%.o: ${SRC_DIR}/%.c
	mkdir -p ${dir $@}
	$(CC) -c $< -o $@ $(CFLAGS) 

${BUILD_DIR}/${TARGET}: ${OBJS}
	$(CC) -o ${TARGET}.bin $^ ${CFLAGS}

.PHONY: flash
flash:
	avrdude -p ${MCU} -c ${PROG_METHOD} -P ${SERIAL_PORT} -b 57600 -U flash:w:${TARGET}.hex:i

.PHONY: clean
clean:
	rm -f ${TARGET}.bin ${TARGET}.hex
	rm -f ${OBJS}

-PHONY: cleandep
cleandep:
	rm -f ${DEPS}

-include ${DEPS}
