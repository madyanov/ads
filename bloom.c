#include "bloom.h"

bloom_t *bloom_new(size_t cap, float fpp) {
    if (!cap || fpp <= 0) {
        return NULL;
    }

    bloom_t *bloom = malloc(sizeof(bloom_t));

    if (!bloom) {
        return NULL;
    }

    size_t size = cap * -1.4427 * log2(fpp) + 1; // 1.4427 == 1/ln(2)
    bits_t *bits = bits_new(size);

    if (!bits) {
        bloom_free(bloom);
        return NULL;
    }

    bloom->bits = bits;
    bloom->size = size;
    bloom->hashes = 0.6931 * size / cap + 1; // 0.6931 == ln(2)
    return bloom;
}

void bloom_free(bloom_t *bloom) {
    if (bloom->bits) {
        free(bloom->bits);
    }

    free(bloom);
}

void bloom_add(bloom_t *bloom, const void *key, size_t len) {
    for (size_t i = 0; i < bloom->hashes; i++) {
        uint32_t hash = murmur3_hash32(key, len, i) % bloom->size;
        bits_set(bloom->bits, hash);
    }
}

int bloom_has(bloom_t *bloom, const void *key, size_t len) {
    for (size_t i = 0; i < bloom->hashes; i++) {
        uint32_t hash = murmur3_hash32(key, len, i) % bloom->size;
        
        if (!bits_test(bloom->bits, hash)) {
            return 0;
        }
    }

    return 1;
}
