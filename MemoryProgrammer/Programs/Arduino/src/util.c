#include "util.h"

uint32_t mini(uint32_t a, uint32_t b) {
    return a <= b ? a : b;
}

bool str_begins_with(char* str, char* pattern) {
    uint32_t max_index = mini(strlen(str), strlen(pattern));
    if(max_index == 0) {
        return false;
    }
    for (uint32_t i = 0; i < max_index; i++) {
        if (str[i] != pattern[i]) {
            return false;
        }
    }
    return true;
}

ISR(TIMER1_COMPA_vect) {
    timer1++;
}

void init_millis(uint64_t f_cpu) {
    uint16_t ms_pattern = f_cpu / (1000 * 8);

    // Set timer1 mode to CTC and prescaler to divide by 8
    set_bit_high(TCCR1B, WGM12);
    set_bit_high(TCCR1B, CS11);

    // Set CTC pattern to match against
    OCR1AH = ms_pattern >> 8;
    OCR1AL = ms_pattern;

    // Enable CTC interrupt
    set_bit_high(TIMSK1, OCIE1A);

    sei();
}

uint32_t millis() {
    uint32_t millis;
    ATOMIC_BLOCK(ATOMIC_FORCEON) {
        millis = timer1;
    }
    return millis;
}
