#ifndef MURMUR3_H
#define MURMUR3_H

#include <stdlib.h>

uint32_t murmur3_hash32(const void *key, size_t len, uint32_t seed);

#endif
