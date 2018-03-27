#ifndef VEC_H
#define VEC_H

#include <stdlib.h>

typedef struct {
    unsigned long cap;
    unsigned long len;
} vec_header_t;

#define vec_push(vec, val) \
    (vec_realloc((void **)&(vec), sizeof(*(vec))) ? \
    ((vec)[vec_header(vec)->len++] = (val), 1) \
    : 0)

#define vec_free(vec) \
    ((vec) != NULL ? (free(vec_header(vec)), (vec) = NULL) : 0)
    
#define vec_last(vec) \
    ((vec)[vec_len(vec) - 1])

#define vec_pop(vec) \
    ((vec)[--vec_header(vec)->len])

int vec_realloc(void **vec, size_t type_size);
unsigned long vec_cap(void *vec);
unsigned long vec_len(void *vec);
void vec_clear(void *vec);
vec_header_t *vec_header(void *vec);

#endif
