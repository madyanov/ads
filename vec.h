#ifndef VEC_H
#define VEC_H

#include <stdlib.h>

typedef struct {
    size_t cap;
    size_t len;
} vec_header_t;

#define vec_push(vec, val) \
    (vec_realloc((void **)&(vec), sizeof(*(vec))) ? \
    ((vec)[vec_header(vec)->len++] = (val), 1) \
    : 0)

#define vec_free(vec) \
    (free(vec_header(vec)), (vec) = NULL)
    
#define vec_last(vec) \
    ((vec)[vec_header(vec)->len - 1])

#define vec_pop(vec) \
    ((vec)[--vec_header(vec)->len])

#define vec_header(vec) \
    ((vec_header_t *)(vec) - 1)

#define vec_cap(vec) \
    ((vec) != NULL ? vec_header(vec)->cap : 0)

#define vec_len(vec) \
    ((vec) != NULL ? vec_header(vec)->len : 0)

int vec_realloc(void **vec, size_t type_size);

#endif
