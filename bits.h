#ifndef BITS_H
#define BITS_H

#include <stdlib.h>
#include <limits.h>

typedef long bits_t;

bits_t *bits_new(size_t size);
void bits_free(bits_t *bits);
void bits_set(bits_t *bits, size_t bit);
void bits_clear(bits_t *bits, size_t bit);
int bits_test(bits_t *bits, size_t bit);

#endif
