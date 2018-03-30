#ifndef VEC_H
#define VEC_H

#include <stdlib.h>
#include <string.h>

#define vec_resize_bits 2 // 1 << vec_resize_bits
#define vec_init_cap 32

#define vec_init(vec) \
    (vec_init_((void **)&(vec), sizeof *(vec)))

#define vec_push(vec, val) \
    (vec_resize_((void **)&(vec), sizeof *(vec)) ? \
    ((vec)[vec_head_(vec)->len++] = (val), 1) \
    : 0)
    
#define vec_last(vec) \
    ((vec)[vec_len(vec) - 1])

#define vec_pop(vec) \
    ((vec)[--vec_head_(vec)->len])

void vec_free(void *vec);
size_t vec_cap(void *vec);
size_t vec_len(void *vec);
void vec_clear(void *vec);

typedef struct {
    size_t cap;
    size_t len;
} vec_head_t;

vec_head_t *vec_head_(void *vec);
int vec_init_(void **vec, size_t tsize);
int vec_resize_(void **vec, size_t tsize);

#endif
