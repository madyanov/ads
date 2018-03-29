#include "vec.h"

#include <stdio.h>

size_t vec_cap(void *vec) {
    return vec ? vec_header_(vec)->cap : 0;
}

size_t vec_len(void *vec) {
    return vec ? vec_header_(vec)->len : 0;
}

void vec_clear(void *vec) {
    if (vec) {
        vec_header_(vec)->len = 0;
    }
}

vec_header_t *vec_header_(void *vec) {
    return (vec_header_t *)vec - 1;
}

int vec_push_alloc_(void **vec, size_t tsize, int zero) {
    vec_header_t *header = NULL;

    if (!*vec) {
        if (!zero) {
            header = malloc(sizeof(vec_header_t) + tsize * vec_min_cap);
        } else {
            header = calloc(1, sizeof(vec_header_t) + tsize * vec_min_cap);
        }

        if (!header) {
            return 0;
        }

        header->cap = vec_min_cap;
        header->len = 0;
    } else {
        header = vec_header_(*vec);

        if (header->len < header->cap) {
            return 1;
        }
        
        size_t cap = header->cap << vec_resize_bits;
        void *new_header = realloc(header, sizeof(vec_header_t) + tsize * cap);

        if (!new_header) {
            return 0;
        }

        header = new_header;
        header->cap = cap;

        if (zero) {
            memset((char *)(header + 1) + tsize * header->len, 0, tsize * (cap - header->len));
        }
    }

    *vec = header + 1;
    return 1;
}

void vec_free_(void **vec) {
    if (*vec) {
        free(vec_header_(*vec));
        *vec = NULL;
    }
}
