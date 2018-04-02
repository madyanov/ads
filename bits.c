#include "bits.h"

static const size_t nbits = sizeof(bits_t) * CHAR_BIT;

bits_t *bits_new(size_t size) {
    if (size < nbits) {
        size = nbits;
    }

    return calloc(1, sizeof(bits_t) * (size / nbits + 1));
}

void bits_free(bits_t *bits) {
    free(bits);
}

void bits_set(bits_t *bits, size_t idx) {
    bits[idx / nbits] |= 1 << (idx % nbits);
}

void bits_clear(bits_t *bits, size_t idx) {
    bits[idx / nbits] &= ~(1 << (idx % nbits));
}

int bits_test(bits_t *bits, size_t idx) {
    return (bits[idx / nbits] & (1 << (idx % nbits))) != 0;
}
