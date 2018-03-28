#ifndef VEC_H
#define VEC_H

#include <stdlib.h>
#include <string.h>

#define vec_min_cap 32

#define vec_push(vec, val) \
    (vec_push_(vec, val, sizeof *(vec), 0))

#define vec_push_zero(vec, val) \
    (vec_push_(vec, val, sizeof *(vec), 1))

#define vec_free(vec) \
    (vec_free_((void **)&(vec)))
    
#define vec_last(vec) \
    ((vec)[vec_len(vec) - 1])

#define vec_pop(vec) \
    ((vec)[--vec_header_(vec)->len])

size_t vec_cap(void *vec);
size_t vec_len(void *vec);
void vec_clear(void *vec);

typedef enum {
    NONE,
    NULLIFY,
    ZERO
} vec_alloc_strat;

typedef struct {
    size_t cap;
    size_t len;
} vec_header_t;

#define vec_push_(vec, val, size, zero) \
    (vec_push_alloc_((void **)&(vec), (size), (zero)) ? \
    ((vec)[vec_header_(vec)->len++] = (val), 1) \
    : 0)

vec_header_t *vec_header_(void *vec);
int vec_push_alloc_(void **vec, size_t tsize, int zero);
void vec_free_(void **vec);

#endif
