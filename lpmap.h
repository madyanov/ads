#ifndef LPMAP_H
#define LPMAP_H

#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <stdio.h>

#define lpmap_load_factor 0.6
#define lpmap_resize_bits 1 // 1 << llmap_resize_bits
#define lpmap_init_cap 16
#define lpmap_key_len 32

#define lpmap_t(T) \
    struct { lpmap_t *m; T t; T *p; }

#define lpmap_init(map) \
    ((map).m = lpmap_new(sizeof (map).t))

#define lpmap_set(map, key, val) \
    ((map).t = (val), lpmap_set_(&(map).m, (key), &(map).t, sizeof (map).t))

#define lpmap_get(map, key) \
    ((map).p = lpmap_get_((map).m, (key), sizeof (map).t))
    
#define lpmap_free(map) \
    (free((map).m))

#define lpmap_cap(map) \
    ((map).m->cap)

#define lpmap_len(map) \
    ((map).m->len)

typedef struct {
    uint32_t hash;
    char key[lpmap_key_len];
    size_t tsize;
    char val[];
} lpmap_node_t;

typedef struct {
    size_t cap;
    size_t len;
    lpmap_node_t buckets[];
} lpmap_t;

lpmap_t *lpmap_new(size_t tsize);
int lpmap_set_(lpmap_t **map, const char *key, void *val, size_t tsize);
void *lpmap_get_(lpmap_t *map, const char *key, size_t tsize);

#endif
