#pragma once

#ifndef F_CPU
#warning "F_CPU undefined"
#endif
#ifndef BAUD
#warning "BAUD undefined"
#endif
#ifndef RX_BUFFER_SIZE
#warning "RX_BUFFER_SIZE undefined"
#endif
#ifndef TX_BUFFER_SIZE
#warning "TX_BUFFER_SIZE undefined"
#endif

#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdint.h>
#include <util/setbaud.h>

#include "defines.h"
#include "util.h"

ISR(USART_RX_vect);

ISR(USART_UDRE_vect);

void init_uart();

void serial_write_byte(uint8_t data);

void serial_write_string(char* str);

void serial_write_string_blocking(char* str);

bool serial_data_avialable();

uint8_t serial_read_byte();

void serial_read_string(char* str, uint8_t buffer_size);

void serial_read_string_blocking(char* str, uint8_t buffer_size);
