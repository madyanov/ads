#include "vec.h"

vec_header_t *vec_header(void *vec) {
    return (vec_header_t *)((vec_header_t *)(vec) - 1);
}

size_t vec_cap(void *vec) {
    if (vec == NULL) {
        return 0;
    }

    return vec_header(vec)->cap;
}

size_t vec_len(void *vec) {
    if (vec == NULL) {
        return 0;
    }

    return vec_header(vec)->len;
}

int vec_expand(void **vec, size_t type_size) {
    vec_header_t *header = vec_header(*vec);

    if (*vec == NULL) {
        header = (vec_header_t *)malloc(sizeof *header + type_size);

        if (header == NULL) {
            return 0;
        }

        header->cap = 1;
        header->len = 0;
    } else {
        if (header->len < header->cap) {
            return 1;
        }
        
        size_t count = header->cap * 2;
        header = (vec_header_t *)realloc(header, sizeof *header + type_size * count);

        if (header == NULL) {
            return 0;
        }

        header->cap = count;
    }

    *vec = header + 1;
    return 1;
}
