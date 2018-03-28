#include <stdlib.h>
#include <stdio.h>

#include "tests.h"

int main() {
    vec_tests();
    bits_tests();
    rk_tests();
    bloom_tests();
    map_tests();

    printf("\nOK.\n");
    return 0;
}
