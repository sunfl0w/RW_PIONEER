#pragma once

#define set_bit_high(vec, bit) (vec |= (1 << bit))
#define clear_bit(vec, bit) (vec &= ~(1 << bit))
#define bit_set(vec, bit) (vec & (1 << bit))
#define set_bits_high(vec, mask) (vec |= mask)
#define clear_bits(vec, mask) (vec &= ~mask)
#define assign_bits(vec1, vec2) (vec1 = vec2)
