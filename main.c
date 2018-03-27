#include <stdlib.h>
#include <assert.h>
#include <limits.h>
#include <string.h>
#include <stdio.h>

#include "vec.h"
#include "bits.h"
#include "rk.h"

void vec_tests() {
    int *vec = NULL;
    assert(vec_cap(vec) == 0);
    assert(vec_len(vec) == 0);

    assert(vec_push(vec, 100) == 1);
    assert(vec_cap(vec) == vec_min_cap);
    assert(vec_len(vec) == 1);

    assert(vec_pop(vec) == 100);
    assert(vec_cap(vec) == vec_min_cap);
    assert(vec_len(vec) == 0);

    assert(vec_push(vec, 100) == 1);
    assert(vec_push(vec, 200) == 1);
    assert(vec_push(vec, 300) == 1);
    assert(vec_cap(vec) == vec_min_cap);
    assert(vec_len(vec) == 3);
    assert(vec[0] == 100);
    assert(vec[1] == 200);
    assert(vec[2] == 300);
    assert(vec_last(vec) == 300);

    for (size_t i = 0; i < vec_min_cap; i++) {
        vec_push(vec, i);
    }

    assert(vec_cap(vec) == vec_min_cap * 2);

    vec_free(vec);
    assert(vec == NULL);

    char *str = NULL;
    vec_push(str, 'H');
    vec_push(str, 'e');
    vec_push(str, 'l');
    vec_push(str, 'l');
    vec_push(str, 'o');
    vec_push(str, '\0');
    assert(strcmp(str, "Hello") == 0);
    vec_free(str);
}

void bits_tests() {
    bits_t *bits = bits_new(1000);
    assert(bits_test(bits, 3) == 0);

    bits_set(bits, 3);
    assert(bits_test(bits, 3) == 1);

    bits_clear(bits, 3);
    assert(bits_test(bits, 3) == 0);

    bits_free(bits);
}

void rk_tests() {
    char *text = "XXXXAXXXXB";
    unsigned long prime = 453126437;

    size_t *occs = NULL;

    rk_search(&occs, text, "XXX");
    assert(vec_len(occs) == 4);
    assert(occs[0] == 0);
    assert(occs[1] == 1);
    assert(occs[2] == 5);
    assert(occs[3] == 6);
    vec_clear(occs);
    
    rk_search(&occs, text, "XXXXA");
    assert(vec_len(occs) == 1);
    assert(occs[0] == 0);
    vec_clear(occs);
    
    rk_search(&occs, text, "XXXXB");
    assert(vec_len(occs) == 1);
    assert(occs[0] == 5);
    vec_free(occs);
    
    rk_search(&occs, text, "XXXXAXXXXB");
    assert(vec_len(occs) == 1);
    assert(occs[0] == 0);
    vec_clear(occs);
    
    rk_search(&occs, text, "CCC");
    assert(vec_len(occs) == 0);
    vec_clear(occs);

    vec_free(occs);
}

int main() {
    vec_tests();
    bits_tests();
    rk_tests();
    printf("\nOK.\n");
    return 0;
}
