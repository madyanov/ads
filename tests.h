#ifndef TEST_H
#define TEST_H

#include <stdlib.h>
#include <assert.h>
#include <limits.h>
#include <string.h>
#include <stdio.h>

#include "vec.h"
#include "bits.h"
#include "rk.h"
#include "bloom.h"
#include "llmap.h"

typedef struct {
    int x;
    int y;
} tests_point_t;

void vec_test_struct(tests_point_t **points) {
    tests_point_t p = { 10, 20 };
    vec_push(*points, p);
}

void vec_tests() {
    long long *vec;
    assert(vec_init(vec) == 1);

    assert(vec_cap(vec) == vec_init_cap);
    assert(vec_len(vec) == 0);

    assert(vec_push(vec, 100) == 1);
    assert(vec_len(vec) == 1);

    assert(vec_pop(vec) == 100);
    assert(vec_len(vec) == 0);

    assert(vec_push(vec, 100) == 1);
    assert(vec_push(vec, 200) == 1);
    assert(vec_push(vec, 300) == 1);
    assert(vec_len(vec) == 3);
    assert(vec[0] == 100);
    assert(vec[1] == 200);
    assert(vec[2] == 300);
    assert(vec_last(vec) == 300);

    for (size_t i = 0; i < vec_init_cap; i++) {
        vec_push(vec, i);
    }

    assert(vec_cap(vec) == vec_init_cap << vec_resize_bits);

    vec_free(vec);

    char *str;
    assert(vec_init(str) == 1);

    vec_push(str, 'H');
    vec_push(str, 'e');
    vec_push(str, 'l');
    vec_push(str, 'l');
    vec_push(str, 'o');
    vec_push(str, '\0');
    assert(strcmp(str, "Hello") == 0);
    vec_free(str);

    char **strs;
    assert(vec_init(strs) == 1);
    
    vec_push(strs, "test");
    assert(vec_len(strs) == 1);
    assert(strs[0] == "test");
    vec_free(strs);

    tests_point_t *points;
    assert(vec_init(points) == 1);
    
    vec_test_struct(&points);
    assert(vec_len(points) == 1);
    assert(points[0].x == 10);
    assert(points[0].y == 20);
    vec_free(points);
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

    size_t *occs;
    vec_init(occs);

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
    vec_clear(occs);
    
    rk_search(&occs, text, "XXXXAXXXXB");
    assert(vec_len(occs) == 1);
    assert(occs[0] == 0);
    vec_clear(occs);
    
    rk_search(&occs, text, "CCC");
    assert(vec_len(occs) == 0);
    vec_clear(occs);

    vec_free(occs);
}

void bloom_tests() {
    bloom_t *bloom = bloom_new(10, 0.1);
    assert(bloom->size == 48);
    assert(bloom->hashes == 4);

    bloom_add(bloom, "a", 1);
    assert(bloom_has(bloom, "a", 1) == 1);
    assert(bloom_has(bloom, "b", 1) == 0);

    bloom_add(bloom, "c", 1);
    assert(bloom_has(bloom, "c", 1) == 1);

    bloom_free(bloom);
}

void llmap_tests() {
    llmap_t(int) imap;
    llmap_init(imap);

    assert(llmap_cap(imap) == llmap_init_cap);
    assert(llmap_len(imap) == 0);

    llmap_set(imap, "k1", 10);
    llmap_set(imap, "k2", 20);
    assert(llmap_len(imap) == 2);
    assert(*llmap_get(imap, "k1") == 10);
    assert(*llmap_get(imap, "k2") == 20);
    assert(llmap_get(imap, "k3") == NULL);

    llmap_set(imap, "k2", 30);
    assert(llmap_len(imap) == 2);
    assert(*llmap_get(imap, "k2") == 30);

    llmap_del(imap, "k1");
    llmap_del(imap, "k3");
    assert(llmap_len(imap) == 1);
    assert(llmap_get(imap, "k1") == NULL);
    assert(*llmap_get(imap, "k2") == 30);
    assert(llmap_get(imap, "k3") == NULL);

    llmap_del(imap, "k2");

    llmap_iter_init(imap);
    assert(llmap_iter_next(imap) == NULL);
    assert(llmap_iter_next(imap) == NULL);
    assert(llmap_iter_next(imap) == NULL);

    for (size_t i = 0; i < llmap_init_cap << llmap_resize_bits; i++) {
        char key[100];
        sprintf(key, "k%zu", i);
        llmap_set(imap, key, i);
        assert(*llmap_get(imap, key) == i);
    }

    assert(llmap_len(imap) == llmap_init_cap << llmap_resize_bits);
    assert(llmap_cap(imap) == llmap_init_cap << llmap_resize_bits << llmap_resize_bits);

    llmap_iter_init(imap);
    llmap_node_t *node = NULL;
    size_t i = 0;
    
    while ((node = llmap_iter_next(imap))) {
        int val = *llmap_node_val(imap, node);
        assert(*llmap_get(imap, node->key) == val);
        i++;
    }

    assert(llmap_len(imap) == i);

    for (size_t i = 0; i < llmap_init_cap << llmap_resize_bits; i++) {
        char key[100];
        sprintf(key, "k%zu", i);
        llmap_del(imap, key);
        assert(llmap_get(imap, key) == NULL);
    }

    assert(llmap_len(imap) == 0);
    assert(llmap_cap(imap) == llmap_init_cap);

    llmap_free(imap);
}

#endif
