#ifndef DHMAP_H
#define DHMAP_H

#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <stdio.h>

#define dhmap_load_factor 0.6
#define dhmap_resize_bits 1 // 1 << llmap_resize_bits
#define dhmap_init_cap 16
#define dhmap_key_len 32

#define dhmap_t(T) \
    struct { dhmap_t *m; T t; T *p; }

#define dhmap_init(map) \
    ((map).m = dhmap_new(dhmap_init_cap, sizeof (map).t))

#define dhmap_set(map, key, val) \
    ((map).t = (val), dhmap_set_(&(map).m, (key), &(map).t, sizeof (map).t))

#define dhmap_get(map, key) \
    ((map).p = dhmap_get_((map).m, (key), sizeof (map).t))

#define dhmap_del(map, key) \
    (dhmap_del_(&(map).m, (key), sizeof (map).t))
    
#define dhmap_free(map) \
    (dhmap_free_((map).m))

#define dhmap_cap(map) \
    ((map).m->cap)

#define dhmap_len(map) \
    ((map).m->len)

typedef enum {
    empty,
    filled,
    deleted
} dhmap_node_state_t;

typedef struct {
    dhmap_node_state_t state;
    uint32_t hash;
    char key[dhmap_key_len];
    char val[];
} dhmap_node_t;

typedef struct {
    size_t cap;
    size_t len;
    dhmap_node_t buckets[];
} dhmap_t;

dhmap_t *dhmap_new(size_t cap, size_t tsize);

void dhmap_free_(dhmap_t *map);
int dhmap_set_(dhmap_t **map, const char *key, void *val, size_t tsize);
void *dhmap_get_(dhmap_t *map, const char *key, size_t tsize);
int dhmap_del_(dhmap_t **map, const char *key, size_t tsize);

#endif
