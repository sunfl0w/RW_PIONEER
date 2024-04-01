#pragma once

#include "defines.h"

#include <avr/interrupt.h>
#include <string.h>
#include <util/atomic.h>

typedef uint8_t bool;
#define true 1
#define false 0

static volatile uint32_t timer1;

uint32_t mini(uint32_t a, uint32_t b);

bool str_begins_with(char* str, char* pattern);

ISR(TIMER1_COMPA_vect);

void init_millis(uint64_t f_cpu);

uint32_t millis();
