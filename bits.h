#ifndef BITS_H
#define BITS_H

#include <stdlib.h>
#include <limits.h>

typedef int bits_t;

bits_t *bits_new(unsigned long size);
void bits_free(bits_t *bits);
void bits_set(bits_t *bits, unsigned long bit);
void bits_clear(bits_t *bits, unsigned long bit);
int bits_test(bits_t *bits, unsigned long bit);

#endif
