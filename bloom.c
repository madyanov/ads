#include "bloom.h"

bloom_t *bloom_new(size_t size, float fpp) {
    if (!size || fpp <= 0) {
        return NULL;
    }

    bloom_t *bloom = malloc(sizeof(bloom_t));

    if (!bloom) {
        return NULL;
    }

    size_t nbits = size * -1.4427 * log2(fpp) + 1;
    bits_t *bits = bits_new(nbits);

    if (!bits) {
        bloom_free(bloom);
        return NULL;
    }

    bloom->bits = bits;
    bloom->nbits = nbits;
    bloom->nhashes = 0.6931 * nbits / size + 1;
    return bloom;
}

void bloom_free(bloom_t *bloom) {
    if (bloom) {
        if (bloom->bits) {
            free(bloom->bits);
        }

        free(bloom);
    }
}

void bloom_add(bloom_t *bloom, const void *key, size_t len) {
    for (size_t i = 0; i < bloom->nhashes; i++) {
        uint32_t hash = murmur3_hash32(key, len, i) % bloom->nbits;
        bits_set(bloom->bits, hash);
    }
}

int bloom_has(bloom_t *bloom, const void *key, size_t len) {
    for (size_t i = 0; i < bloom->nhashes; i++) {
        uint32_t hash = murmur3_hash32(key, len, i) % bloom->nbits;
        
        if (!bits_test(bloom->bits, hash)) {
            return 0;
        }
    }

    return 1;
}
