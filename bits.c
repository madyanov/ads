#include "bits.h"

const long long_bits = sizeof(bits_t) * CHAR_BIT;

bits_t *bits_new(size_t size) {
    if (size < long_bits) {
        size = long_bits;
    }

    return calloc(1, sizeof(bits_t) * size / long_bits);
}

void bits_free(bits_t *bits) {
    free(bits);
}

void bits_set(bits_t *bits, size_t idx) {
    bits[idx / long_bits] |= 1 << (idx % long_bits);
}

void bits_clear(bits_t *bits, size_t idx) {
    bits[idx / long_bits] &= ~(1 << (idx % long_bits));
}

int bits_test(bits_t *bits, size_t idx) {
    return (bits[idx / long_bits] & (1 << (idx % long_bits))) != 0;
}
