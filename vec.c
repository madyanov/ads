#include "vec.h"

uint64_t vec_cap(void *vec) {
    return vec != NULL ? vec_header(vec)->cap : 0;
}

uint64_t vec_len(void *vec) {
    return vec != NULL ? vec_header(vec)->len : 0;
}

vec_header_t *vec_header(void *vec) {
    return (vec_header_t *)(vec) - 1;
}

int vec_realloc(void **vec, size_t type_size) {
    vec_header_t *header = vec_header(*vec);

    if (*vec == NULL) {
        header = malloc(sizeof *header + type_size);

        if (header == NULL) {
            return 0;
        }

        header->cap = 1;
        header->len = 0;
    } else {
        if (header->len < header->cap) {
            return 1;
        }
        
        uint64_t cap = header->cap * 2;
        header = realloc(header, sizeof *header + type_size * cap);

        if (header == NULL) {
            return 0;
        }

        header->cap = cap;
    }

    *vec = header + 1;
    return 1;
}
