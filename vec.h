#ifndef VEC_H
#define VEC_H

#include <stdlib.h>
#include <string.h>

#define vec_resize_bits 2 // 1 << vec_resize_bits
#define vec_init_cap 32

#define vec_push(vec, val) \
    (vec_push_alloc_((void **)&(vec), sizeof *(vec)) ? \
    ((vec)[vec_head_(vec)->len++] = (val), 1) \
    : 0)

#define vec_free(vec) \
    (vec_free_((void **)&(vec)))
    
#define vec_last(vec) \
    ((vec)[vec_len(vec) - 1])

#define vec_pop(vec) \
    ((vec)[--vec_head_(vec)->len])

size_t vec_cap(void *vec);
size_t vec_len(void *vec);
void vec_clear(void *vec);

typedef struct {
    size_t cap;
    size_t len;
} vec_head_t;

vec_head_t *vec_head_(void *vec);
int vec_push_alloc_(void **vec, size_t tsize);
void vec_free_(void **vec);

#endif
