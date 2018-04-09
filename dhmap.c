#include "dhmap.h"

uint32_t dhmap_hash1(const char *key) {
    uint32_t hash = 5381;

    while (*key) {
        hash = ((hash << 5) + hash) ^ *key++;
    }

    return hash;
}

uint32_t dhmap_hash2(const char *key) {
    uint32_t hash = 0;

    while (*key) {
        hash = ((hash << 5) - hash) ^ *key++;
    }

    return hash;
}

// ==========
// nodes

void dhmap_node_set(dhmap_node_t *node, uint32_t hash, const char *key, void *val, size_t tsize) {
    node->state = dhmap_filled;
    node->hash = hash;
    memcpy(node->key, key, dhmap_key_len);
    memcpy(node->val, val, tsize);
}

// ==========
// map

dhmap_t *dhmap_new(size_t cap, size_t tsize) {
    dhmap_t *map = calloc(1, offsetof(dhmap_t, buckets) + (sizeof(dhmap_node_t) + tsize) * cap);

    if (!map) {
        return NULL;
    }

    map->cap = cap;
    map->len = 0;
    return map;
}

void dhmap_free_(dhmap_t *map) {
    free(map);
}

size_t dhmap_mod(dhmap_t *map, uint32_t hash) {
    return hash & (map->cap - 1);
}

dhmap_node_t *dhmap_node_at(dhmap_t *map, size_t idx, size_t tsize) {
    return (dhmap_node_t *)((char *)(map->buckets + idx) + idx * tsize);
}

dhmap_node_t *dhmap_node(dhmap_t *map, const char *key, size_t tsize, int deleted) {
    uint32_t hash = dhmap_hash1(key);
    size_t idx = dhmap_mod(map, hash);
    size_t step = dhmap_mod(map, dhmap_hash2(key));
    dhmap_node_t *node = NULL;

    for (;;) {
        node = dhmap_node_at(map, idx, tsize);

        if (node->state == dhmap_empty) {
            return node;
        }

        if (deleted && node->state == dhmap_deleted) {
            return node;
        }

        if (node->hash == hash && strncmp(key, node->key, dhmap_key_len) == 0) {
            return node;
        }

        idx = dhmap_mod(map, idx + step);
    }

    return NULL;
}

void dhmap_add(dhmap_t *map, uint32_t hash, const char *key, void *val, size_t tsize) {
    dhmap_node_t *node = dhmap_node(map, key, tsize, 1);

    if (node->state != dhmap_filled) {
        map->len++;
    }

    node->state = dhmap_filled;
    node->hash = hash;
    memcpy(node->key, key, dhmap_key_len);
    memcpy(node->val, val, tsize);
}

int dhmap_resize(dhmap_t **map, size_t tsize) {
    size_t cap = 0;

    if ((*map)->len >= (*map)->cap * dhmap_load_factor) {
        cap = (*map)->cap << dhmap_resize_bits;
    } else if ((*map)->cap > dhmap_init_cap && (*map)->len <= ((*map)->cap >> dhmap_resize_bits) * dhmap_load_factor) {
        cap = (*map)->cap >> dhmap_resize_bits;
    } else {
        return 1;
    }

    dhmap_t *new_map = dhmap_new(cap, tsize);

    if (!new_map) {
        return 0;
    }

    dhmap_node_t *node = NULL;

    for (size_t i = 0; i < (*map)->cap; i++) {
        node = dhmap_node_at(*map, i, tsize);

        if (node->state == dhmap_filled) {
            dhmap_add(new_map, node->hash, node->key, node->val, tsize);
        }
    }

    dhmap_free_(*map);

    *map = new_map;
    return 1;
}

int dhmap_set_(dhmap_t **map, const char *key, void *val, size_t tsize) {
    if (dhmap_resize(map, tsize)) {
        dhmap_add(*map, dhmap_hash1(key), key, val, tsize);
        return 1;
    }

    return 0;
}

void *dhmap_get_(dhmap_t *map, const char *key, size_t tsize) {
    dhmap_node_t *node = dhmap_node(map, key, tsize, 0);

    if (node && node->state == dhmap_filled) {
        return node->val;
    }
    
    return NULL;
}

void dhmap_del_(dhmap_t **map, const char *key, size_t tsize) {
    dhmap_node_t *node = dhmap_node(*map, key, tsize, 0);

    if (node && node->state == dhmap_filled) {
        node->state = dhmap_deleted;
        (*map)->len--;
        dhmap_resize(map, tsize);
    }
}

// ==========
// iterator

dhmap_node_t *dhmap_iter_next_(dhmap_t *map, dhmap_iter_t *iter, size_t tsize) {
    dhmap_node_t *node = NULL;

    do {
        if (iter->idx >= map->cap) {
            return NULL;
        }
        
        node = dhmap_node_at(map, iter->idx, tsize);
        iter->idx++;
    } while (node->state != dhmap_filled);

    return node;
}
