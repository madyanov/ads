#include "vec.h"

size_t vec_cap(void *vec) {
    return vec != NULL ? vec_header(vec)->cap : 0;
}

size_t vec_len(void *vec) {
    return vec != NULL ? vec_header(vec)->len : 0;
}

vec_header_t *vec_header(void *vec) {
    return (vec_header_t *)(vec) - 1;
}

void vec_clear(void *vec) {
    if (vec != NULL) {
        vec_header(vec)->len = 0;
    }
}

int vec_resize(void **vec, size_t size) {
    vec_header_t *header = vec_header(*vec);

    if (*vec == NULL) {
        header = malloc(sizeof *header + size * vec_min_cap);

        if (header == NULL) {
            return 0;
        }

        header->cap = vec_min_cap;
        header->len = 0;
    } else {
        if (header->len < header->cap) {
            return 1;
        }
        
        size_t cap = header->cap * 2;
        void *new_header = realloc(header, sizeof *header + size * cap);

        if (new_header == NULL) {
            return 0;
        }

        header = new_header;
        header->cap = cap;
    }

    *vec = header + 1;
    return 1;
}
