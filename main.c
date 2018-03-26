#include <stdlib.h>
#include <assert.h>
#include <limits.h>

#include "vec.h"
#include "bits.h"

void vec_tests() {
    int *vec = NULL;
    assert(vec_cap(vec) == 0);
    assert(vec_len(vec) == 0);

    assert(vec_push(vec, 100) == 1);
    assert(vec_cap(vec) == 1);
    assert(vec_len(vec) == 1);

    assert(vec_pop(vec) == 100);
    assert(vec_cap(vec) == 1);
    assert(vec_len(vec) == 0);

    assert(vec_push(vec, 100) == 1);
    assert(vec_push(vec, 200) == 1);
    assert(vec_push(vec, 300) == 1);
    assert(vec_cap(vec) == 4);
    assert(vec_len(vec) == 3);
    assert(vec[0] == 100);
    assert(vec[1] == 200);
    assert(vec[2] == 300);
    assert(vec_last(vec) == 300);

    vec_free(vec);
    assert(vec == NULL);
}

void bits_tests() {
    bits_t *bits = bits_new(1);
    assert(bits->size == sizeof(int) * CHAR_BIT);
    assert(bits_test(bits, 3) == 0);

    assert(bits_set(bits, 3) == 1);
    assert(bits_test(bits, 3) == 1);

    assert(bits_clear(bits, 3) == 1);
    assert(bits_test(bits, 3) == 0);

    bits_free(bits);
}

int main() {
    vec_tests();
    bits_tests();
    return 0;
}