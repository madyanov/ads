#include "bits.h"

int bits_int = sizeof(int) * CHAR_BIT;

bits_t *bits_new(unsigned long size) {
    if (size < bits_int) {
        size = bits_int;
    }

    return calloc(1, sizeof(bits_t) * size / bits_int);
}

void bits_free(bits_t *bits) {
    if (bits != NULL) {
        free(bits);
    }
}

void bits_set(bits_t *bits, unsigned long bit) {
    bits[bit / bits_int] |= 1 << (bit % bits_int);
}

void bits_clear(bits_t *bits, unsigned long bit) {
    bits[bit / bits_int] &= ~(1 << (bit % bits_int));
}

int bits_test(bits_t *bits, unsigned long bit) {
    return (bits[bit / bits_int] & (1 << (bit % bits_int))) != 0;
}
