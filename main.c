#include <stdlib.h>
#include <assert.h>

#include "vec.h"

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

    vec_free(vec);
    assert(vec == NULL);
}

int main() {
    vec_tests();
    return 0;
}
