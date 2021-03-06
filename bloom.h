#ifndef BLOOM_H
#define BLOOM_H

#include <stdlib.h>
#include <math.h>

#include "bits.h"
#include "murmur3.h"

typedef struct {
    bits_t *bits;
    size_t size;
    size_t hashes;
} bloom_t;

bloom_t *bloom_new(size_t cap, float fpp);
void bloom_free(bloom_t *bloom);
void bloom_add(bloom_t *bloom, const void *key, size_t len);
int bloom_has(bloom_t *bloom, const void *key, size_t len);

#endif
