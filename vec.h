#ifndef VEC_H
#define VEC_H

#include <stdlib.h>

typedef struct {
    uint64_t cap;
    uint64_t len;
} vec_header_t;

#define vec_push(vec, val) \
    (vec_realloc((void **)&(vec), sizeof(*(vec))) ? \
    ((vec)[vec_header(vec)->len++] = (val), 1) \
    : 0)

#define vec_free(vec) \
    (free(vec_header(vec)), (vec) = NULL)
    
#define vec_last(vec) \
    ((vec)[vec_len(vec) - 1])

#define vec_pop(vec) \
    ((vec)[--vec_header(vec)->len])

int vec_realloc(void **vec, size_t type_size);
uint64_t vec_cap(void *vec);
uint64_t vec_len(void *vec);
vec_header_t *vec_header(void *vec);

#endif
