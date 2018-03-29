#include "vec.h"

size_t vec_cap(void *vec) {
    return vec ? vec_head_(vec)->cap : 0;
}

size_t vec_len(void *vec) {
    return vec ? vec_head_(vec)->len : 0;
}

void vec_clear(void *vec) {
    if (!vec) {
        return;
    }
    
    vec_head_(vec)->len = 0;
}

vec_head_t *vec_head_(void *vec) {
    return (vec_head_t *)vec - 1;
}

int vec_init_(void **vec, size_t tsize) {
    vec_head_t *head = malloc(sizeof(vec_head_t) + tsize * vec_init_cap);

    if (!head) {
        return 0;
    }

    head->cap = vec_init_cap;
    head->len = 0;

    *vec = head + 1;
    return 1;
}

int vec_resize_(void **vec, size_t cap, size_t tsize) {
    vec_head_t *head = vec_head_(*vec);
    void *new_head = realloc(head, sizeof(vec_head_t) + tsize * cap);

    if (!new_head) {
        return 0;
    }

    head = new_head;
    head->cap = cap;

    *vec = head + 1;
    return 1;
}

int vec_push_alloc_(void **vec, size_t tsize) {
    if (*vec) {
        vec_head_t *head = vec_head_(*vec);

        if (head->len < head->cap) {
            return 1;
        }
        
        size_t cap = head->cap << vec_resize_bits;
        return vec_resize_(vec, cap, tsize);
    }

    return vec_init_(vec, tsize);
}

void vec_free_(void **vec) {
    if (!*vec) {
        return;
    }

    free(vec_head_(*vec));
    *vec = NULL;
}
