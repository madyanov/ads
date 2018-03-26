#include "bits.h"
#include <stdio.h>
#include <string.h>

int bits_int = sizeof(int) * CHAR_BIT;

bits_t *bits_new(uint64_t size) {
    if (size < bits_int) {
        size = bits_int;
    }

    bits_t *bits = calloc(1, sizeof(bits_t) + sizeof(int) * size / bits_int);

    if (bits == NULL) {
        return NULL;
    }

    bits->size = size;
    bits->bits = (int *)(bits + 1);
    return bits;
}

void bits_free(bits_t *bits) {
    free(bits);
}

int bits_set(bits_t *bits, uint64_t bit) {
    if (bit >= bits->size) {
        return 0;
    }

    (bits->bits)[bit / bits_int] |= 1 << (bit % bits_int);
    return 1;
}

int bits_clear(bits_t *bits, uint64_t bit) {
    if (bit >= bits->size) {
        return 0;
    }

    (bits->bits)[bit / bits_int] &= ~(1 << (bit % bits_int));
    return 1;
}

int bits_test(bits_t *bits, uint64_t bit) {
    if (bit >= bits->size) {
        return 0;
    }

    return ((bits->bits)[bit / bits_int] & (1 << (bit % bits_int))) != 0;
}
