#include "bloom.h"

bloom_t *bloom_new(size_t count, float fpp) {
    if (count == 0 || fpp <= 0) {
        return NULL;
    }

    bloom_t *bloom = malloc(sizeof(bloom_t));

    if (bloom == NULL) {
        return NULL;
    }

    size_t size = count * -1.4427 * log2(fpp) + 1;
    bits_t *bits = bits_new(size);

    if (bits == NULL) {
        return NULL;
    }

    bloom->bits = bits;
    bloom->size = size;
    bloom->hashes = 0.6931 * size / count + 1;
    return bloom;
}

void bloom_free(bloom_t *bloom) {
    if (bloom != NULL) {
        if (bloom->bits != NULL) {
            free(bloom->bits);
        }

        free(bloom);
    }
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
        
        if (bits_test(bloom->bits, hash) == 0) {
            return 0;
        }
    }

    return 1;
}
