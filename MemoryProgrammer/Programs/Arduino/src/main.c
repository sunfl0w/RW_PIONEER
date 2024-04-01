// Pinout based on Arduino pin naming
// Serial data => PD2/D2
// Serial clock => PD3/D3
// Serial Latch => PD4/D4
// Write enable => PD5/D5
// Memory IO (LSB to MSB) => PD6 to PB5 / D6 to D13

#include "defines.h"
#include "stdlib.h"
#include "uart.h"
#include "util.h"

#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <util/delay.h>
#include <util/setbaud.h>

typedef enum IOMode { INPUT = 0, OUTPUT = 1 } IOMode;

typedef enum Operation { Program, Erase, MemDump, InvalidOperation } Operation;

typedef enum Target { AT28C64, AT28C128, AT28C256, SST39SF010A, SST39SF020A, SST39SF040, InvalidTarget } Target;

uint32_t get_target_max_address(Target target) {
    if (target == AT28C64) {
        return 1L << 13;
    } else if (target == AT28C128) {
        return 1L << 14;
    } else if (target == AT28C256) {
        return 1L << 15;
    } else if (target == SST39SF010A) {
        return 1L << 17;
    } else if (target == SST39SF020A) {
        return 1L << 18;
    } else if (target == SST39SF040) {
        return 1L << 19;
    } else {
        return -1L;
    }
}

// Set memory io pins into INPUT or OUTPUT mode
void set_io_mode(IOMode mode) {
    if (mode == INPUT) {
        clear_bits(DDRD, 0b11000000);
        clear_bits(DDRB, 0b00111111);
    } else if (mode == OUTPUT) {
        set_bits_high(DDRD, 0b11000000);
        set_bits_high(DDRB, 0b00111111);
    }
}

void set_target_memory_address(uint16_t address, bool enable_output) {
    uint32_t address_data = address;
    if (!enable_output) {
        address_data |= 1L << 19;
    }
    // Set serial clock low
    clear_bit(PORTD, 1 << 3);

    // Shift out data (20 bits) from MSB to LSB
    for (int i = 19; i >= 0; i--) {
        // Put next bit on serial data output pin
        uint32_t bit_data = (address_data << (31 - i)) >> 31;
        if (bit_data == 1) {
            set_bit_high(PORTD, 1 << 2);
        } else {
            clear_bit(PORTD, 1 << 2);
        }
        // Transition serial clock from low to high to low
        set_bit_high(PORTD, 1 << 3);
        clear_bit(PORTD, 1 << 3);
    }

    // Latch address output
    clear_bit(PORTD, 1 << 4);
    set_bit_high(PORTD, 1 << 4);
    clear_bit(PORTD, 1 << 4);
    _delay_us(10.0);
}

void set_out_data(uint8_t data) {
    clear_bits(PORTD, 0b11000000);
    set_bits_high(PORTD, (data & 0b00000011) << 6);
    clear_bits(PORTB, 0b00111111);
    set_bits_high(PORTB, (data & 0b11111100) >> 2);
}

void write_to_target(Target target, uint32_t address, uint8_t data) {
    if (target == AT28C64 || target == AT28C128 || target == AT28C256) {
        set_bit_high(PORTD, 1 << 5);
        set_target_memory_address(address, false);
        set_io_mode(OUTPUT);
        set_out_data(data);
        clear_bit(PORTD, 1 << 5);
        set_bit_high(PORTD, 1 << 5);
        _delay_ms(1);
    } else if (target == SST39SF010A || target == SST39SF020A || target == SST39SF040) {
        set_bit_high(PORTD, 1 << 5);
        set_io_mode(OUTPUT);

        set_target_memory_address(0x5555, false);
        set_out_data(0xaa);
        set_bit_high(PORTD, 1 << 5);
        clear_bit(PORTD, 1 << 5);
        set_bit_high(PORTD, 1 << 5);

        set_target_memory_address(0x2aaa, false);
        set_out_data(0x55);
        set_bit_high(PORTD, 1 << 5);
        clear_bit(PORTD, 1 << 5);
        set_bit_high(PORTD, 1 << 5);

        set_target_memory_address(0x5555, false);
        set_out_data(0xa0);
        set_bit_high(PORTD, 1 << 5);
        clear_bit(PORTD, 1 << 5);
        set_bit_high(PORTD, 1 << 5);

        set_target_memory_address(address, false);
        set_out_data(data);
        set_bit_high(PORTD, 1 << 5);
        clear_bit(PORTD, 1 << 5);
        set_bit_high(PORTD, 1 << 5);
    }
}

uint8_t read_from_target(uint32_t address) {
    set_io_mode(OUTPUT);
    set_target_memory_address(address, true);

    return ((PIND & 0b11000000) >> 6) | ((PINB & 0b00111111) << 2);
}

void erase_target(Target target, uint32_t range) {
    long max_address = mini(get_target_max_address(target), range);
    if (target == AT28C64 || target == AT28C128 || target == AT28C256) {
        for (uint32_t address = 0; address < max_address; address++) {
            write_to_target(target, address, 0);
        }
    } else if (target == AT28C64 || target == AT28C128 || target == AT28C256) {
        set_bit_high(PORTD, 1 << 5);
        set_io_mode(OUTPUT);

        set_target_memory_address(0x5555, false);
        set_out_data(0xaa);
        set_bit_high(PORTD, 1 << 5);
        clear_bit(PORTD, 1 << 5);
        set_bit_high(PORTD, 1 << 5);

        set_target_memory_address(0x2aaa, false);
        set_out_data(0x55);
        set_bit_high(PORTD, 1 << 5);
        clear_bit(PORTD, 1 << 5);
        set_bit_high(PORTD, 1 << 5);

        set_target_memory_address(0x5555, false);
        set_out_data(0x80);
        set_bit_high(PORTD, 1 << 5);
        clear_bit(PORTD, 1 << 5);
        set_bit_high(PORTD, 1 << 5);

        set_target_memory_address(0x5555, false);
        set_out_data(0xaa);
        set_bit_high(PORTD, 1 << 5);
        clear_bit(PORTD, 1 << 5);
        set_bit_high(PORTD, 1 << 5);

        set_target_memory_address(0x2aaa, false);
        set_out_data(0x55);
        set_bit_high(PORTD, 1 << 5);
        clear_bit(PORTD, 1 << 5);
        set_bit_high(PORTD, 1 << 5);

        set_target_memory_address(0x5555, false);
        set_out_data(0x10);
        set_bit_high(PORTD, 1 << 5);
        clear_bit(PORTD, 1 << 5);
        set_bit_high(PORTD, 1 << 5);

        _delay_ms(100);
    }
}

void serial_send_memory_data(uint32_t max_address) {
    for (uint32_t base = 0; base < max_address; base += 16) {
        uint8_t data[16];
        for (uint32_t offset = 0; offset < 16; offset++) {
            data[offset] = read_from_target(base + offset);
        }

        char buffer[100];
        sprintf(buffer, "%06lx:  %02x %02x %02x %02x %02x %02x %02x %02x   %02x %02x %02x %02x %02x %02x %02x %02x", base, data[0], data[1], data[2], data[3], data[4], data[5], data[6], data[7], data[8], data[9],
                data[10], data[11], data[12], data[13], data[14], data[15]);

        serial_write_string_blocking(buffer);
    }
}

Operation serial_get_operation() {
    serial_write_string_blocking("Send operation");

    char buffer[32];
    serial_read_string_blocking(buffer, 32);

    if (str_begins_with(buffer, "Program")) {
        serial_write_string_blocking("Operation Program set");
        return Program;
    } else if (str_begins_with(buffer, "MemoryDump")) {
        serial_write_string_blocking("Operation MemoryDump set");
        return MemDump;
    } else if (str_begins_with(buffer, "Erase")) {
        serial_write_string_blocking("Operation Erase set");
        return Erase;
    } else {
        serial_write_string_blocking("Operation invalid");
        return InvalidOperation;
    }
}

int serial_get_operation_range() {
    serial_write_string_blocking("Send operation range");
    char buffer[32];
    serial_read_string_blocking(buffer, 32);
    return strtol(buffer, NULL, 10);
}

Target serial_get_target() {
    serial_write_string_blocking("Send target");

    char buffer[32];
    serial_read_string_blocking(buffer, 32);

    if (str_begins_with(buffer, "AT28C64")) {
        serial_write_string_blocking("Target AT28C64 set");
        return AT28C64;
    } else if (str_begins_with(buffer, "AT28C128")) {
        serial_write_string_blocking("Target AT28C128 set");
        return AT28C128;
    }
    if (str_begins_with(buffer, "AT28C256")) {
        serial_write_string_blocking("Target AT28C256 set");
        return AT28C256;
    }
    if (str_begins_with(buffer, "SST39SF010A")) {
        serial_write_string_blocking("Target SST39SF010A set");
        return SST39SF010A;
    }
    if (str_begins_with(buffer, "SST39SF020A")) {
        serial_write_string_blocking("Target SST39SF020A set");
        return SST39SF020A;
    }
    if (str_begins_with(buffer, "SST39SF040")) {
        serial_write_string_blocking("Target SST39SF040 set");
        return SST39SF040;
    } else {
        serial_write_string_blocking("Target invalid");
        return InvalidTarget;
    }
}

void serial_send_memory_dump(int max_address) {
    serial_write_string_blocking("MemoryDump");
    serial_send_memory_data(max_address);
}

int main() {
    init_uart();
    init_millis(F_CPU);

    set_bit_high(PORTD, 1 << 5);
    set_bits_high(DDRD, 0b00111100);

    serial_write_string("Starting RW_FLASH_MICRO");

    Operation operation = serial_get_operation();
    if (operation == InvalidOperation) {
        serial_write_string_blocking("Sent operation invalid. Aborting");
        return -1;
    }

    _delay_ms(1000);

    int range = serial_get_operation_range();
    if (range == -1) {
        serial_write_string_blocking("Sent operation range invalid. Aborting");
        return -1;
    }

    _delay_ms(1000);

    Target target = serial_get_target();
    if (target == InvalidTarget) {
        serial_write_string_blocking("Sent target invalid. Aborting");
        return -1;
    }

    _delay_ms(1000);

    if (operation == Erase) {
        serial_write_string_blocking("Erasing memory");
        erase_target(target, range);
        serial_write_string_blocking("Erasing completed");
        serial_write_string_blocking("Testing memory");

        bool test_fail = false;
        int max_address = get_target_max_address(target);
        for (int i = 0; i < max_address; i++) {
            if (read_from_target(i) != 0) {
                test_fail = true;
                break;
            }
        }
        if (test_fail) {
            serial_write_string_blocking("Memory test failed");
        } else {
            serial_write_string_blocking("Memory test successful");
        }
    } else if (operation == MemDump) {
        serial_send_memory_dump(range);
    } else if (operation == Program) {
        serial_write_string_blocking("Starting programming operation");
        int time_since_last_data = 0;
        int current_address = 0;
        _delay_ms(1000);

        serial_write_string_blocking("Send next byte");

        int max_address = get_target_max_address(target);
        bool programming_failed = false;

        while (time_since_last_data < SERIAL_COMM_TIMEOUT && current_address < max_address) {
            int now = millis();
            if (serial_data_avialable()) {
                uint8_t data = serial_read_byte();

                if (current_address % 1024 == 0) {
                    char out_buffer[32];
                    sprintf(out_buffer, "Byte %x received", current_address);
                    serial_write_string_blocking(out_buffer);
                }

                write_to_target(target, current_address, data);

                if (read_from_target(current_address) != data) {
                    char out_buffer[32];
                    sprintf(out_buffer, "Programming at %x failed", current_address);
                    programming_failed = true;
                    break;
                }

                current_address++;
                time_since_last_data = 0;
                serial_write_string_blocking("Send next byte");
            } else {
                time_since_last_data += millis() - now;
            }
        }

        if (!programming_failed) {
            serial_write_string_blocking("No more data detected");
        }
    }
    serial_write_string_blocking("Operation completed");
    return 0;
}
