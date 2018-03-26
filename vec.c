#include "vec.h"

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
        
        size_t cap = header->cap * 2;
        header = realloc(header, sizeof *header + type_size * cap);

        if (header == NULL) {
            return 0;
        }

        header->cap = cap;
    }

    *vec = header + 1;
    return 1;
}
