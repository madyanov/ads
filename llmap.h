#ifndef LLMAP_H
#define LLMAP_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "murmur3.h"

#define llmap_load_factor 0.5 // resize map if len >= cap * llmap_load_factor
#define llmap_resize_bits 2 // 1 << llmap_resize_bits
#define llmap_init_cap 32

#define llmap_t(T) \
    struct { llmap_t *m; T tmp; T *ptr; llmap_iter_t iter; }

#define llmap_init(map) \
    ((map).m = llmap_new())

#define llmap_set(map, key, val) \
    ((map).tmp = (val), llmap_set_(&(map).m, (key), &(map).tmp, sizeof (map).tmp))

#define llmap_get(map, key) \
    ((map).ptr = llmap_get_((map).m, (key)))

#define llmap_del(map, key) \
    (llmap_del_(&(map).m, (key)))

#define llmap_free(map) \
    (llmap_free_((map).m))

#define llmap_cap(map) \
    ((map).m->cap)//

#define llmap_len(map) \
    ((map).m->len)

#define llmap_iter_init(map) \
    (memset(&(map).iter, 0, sizeof (map).iter))

#define llmap_iter_next(map) \
    (llmap_iter_next_((map).m, &(map).iter))

#define llmap_node_val(map, node) \
    ((map).ptr = llmap_node_val_((node), 0))

typedef struct llmap_node_t {
    struct llmap_node_t *next;
    uint32_t hash;
} llmap_node_t;

typedef struct {
    size_t cap;
    size_t len;
} llmap_t;

typedef struct {
    size_t idx;
    llmap_node_t *node;
} llmap_iter_t;

llmap_t *llmap_new();
char *llmap_node_key(llmap_node_t *node);

int llmap_set_(llmap_t **map, const char *key, void *val, size_t vsize);
void *llmap_get_(llmap_t *map, const char *key);
void llmap_del_(llmap_t **map, const char *key);
void llmap_free_(llmap_t *map);

llmap_node_t *llmap_iter_next_(llmap_t *map, llmap_iter_t *iter);
void *llmap_node_val_(llmap_node_t *node, size_t klen);

#endif
