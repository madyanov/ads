#include "llmap.h"

// https://stackoverflow.com/a/1147232/1558529
static const size_t primes[] = {
    1,
    2,
    3,
    7,
    13,
    31,
    61,
    127,
    251,
    509,
    1021,
    2039,
    4093,
    8191,
    16381,
    32749,
    65521,
    131071,
    262139,
    524287,
    1048573,
    2097143,
    4194301,
    8388593,
    16777213,
    33554393,
    67108859,
    134217689,
    268435399,
    536870909,
    1073741789,
    2147483647
};

// ==========
// nodes

uint32_t llmap_hash(const char *key) {
    return murmur3_hash32(key, strlen(key), 0);
}

void *llmap_node_val_(llmap_node_t *node, size_t klen) {
    return node->key + (klen ? klen : (strlen(node->key) + 1));
}

llmap_node_t *llmap_node_new(const char *key, void *val, size_t vsize) {
    size_t klen = strlen(key);
    llmap_node_t *node = malloc(sizeof(llmap_node_t) + klen + vsize);

    if (!node) {
        return NULL;
    }

    memcpy(node->key, key, klen + 1);
    memcpy(llmap_node_val_(node, klen + 1), val, vsize);

    node->next = NULL;
    node->hash = llmap_hash(key);
    return node;
}

void llmap_node_free(llmap_node_t *node) {
    free(node);
}

// ==========
// map

llmap_t *llmap_new() {
    llmap_t *map = calloc(1, sizeof(llmap_t) + sizeof(llmap_node_t *) * llmap_init_cap);

    if (!map) {
        return NULL;
    }

    map->cap = llmap_init_cap;
    map->len = 0;
    return map;
}

llmap_node_t **llmap_buckets(llmap_t *map) {
    return (llmap_node_t **)(map + 1);
}

size_t llmap_idx(llmap_t *map, uint32_t hash) {
    size_t prime = log2(map->cap);
    return hash % primes[prime];
}

llmap_node_t **llmap_get_node(llmap_t *map, const char *key) {
    uint32_t hash = llmap_hash(key);
    llmap_node_t **node = &llmap_buckets(map)[llmap_idx(map, hash)];

    while (*node) {
        if ((*node)->hash == hash && strcmp(key, (*node)->key) == 0) {
            return node;
        }

        node = &(*node)->next;
    }

    return NULL;
}

void llmap_add_node(llmap_t *map, llmap_node_t *node) {
    llmap_node_t **buckets = llmap_buckets(map);
    size_t idx = llmap_idx(map, node->hash);
    node->next = buckets[idx];
    buckets[idx] = node;
}

llmap_t *llmap_resize(llmap_t *map) {
    size_t cap = 0;

    // grow, shrink, or do nothing
    if (map->len >= map->cap * llmap_load_factor) {
        cap = map->cap << llmap_resize_bits;
    } else if (map->cap > llmap_init_cap && map->len <= (map->cap >> llmap_resize_bits) * llmap_load_factor) {
        cap = map->cap >> llmap_resize_bits;
    } else {
        return map;
    }

    llmap_node_t **buckets = llmap_buckets(map);
    llmap_node_t *node, *next, *first = NULL;

    for (size_t i = 0; i < map->cap; i++) {
        node = buckets[i];

        while (node) {
            next = node->next;
            node->next = first;
            first = node;
            node = next;
        }
    }

    llmap_t *new_map = realloc(map, sizeof(llmap_t) + sizeof(llmap_node_t *) * cap);

    if (!new_map) {
        return NULL;
    }

    memset(new_map + 1, 0, sizeof(llmap_node_t *) * cap);
    new_map->cap = cap;
    node = first;

    while (node) {
        next = node->next;
        llmap_add_node(new_map, node);
        node = next;
    }

    return new_map;
}

int llmap_set_(llmap_t **map, const char *key, void *val, size_t vsize) {
    llmap_node_t **node = llmap_get_node(*map, key);

    if (node) {
        memcpy(llmap_node_val_(*node, 0), val, vsize);
        return 1;
    }

    llmap_node_t *new_node = llmap_node_new(key, val, vsize);

    if (!new_node) {
        return 0;
    }

    *map = llmap_resize(*map);
    llmap_add_node(*map, new_node);
    (*map)->len++;
    return 1;
}

void *llmap_get_(llmap_t *map, const char *key) {
    llmap_node_t **node = llmap_get_node(map, key);

    if (node) {
        return llmap_node_val_(*node, 0);
    }

    return NULL;
}

void llmap_del_(llmap_t **map, const char *key) {
    llmap_node_t **node = llmap_get_node(*map, key);

    if (node) {
        llmap_node_t *next = (*node)->next;
        llmap_node_free(*node);
        *node = next;
        (*map)->len--;
        *map = llmap_resize(*map);
    }
}

void llmap_free_(llmap_t *map) {
    llmap_node_t **buckets = llmap_buckets(map);
    llmap_node_t *node, *next = NULL;

    for (size_t i = 0; i < map->cap; i++) {
        node = buckets[i];

        while (node) {
            next = node->next;
            llmap_node_free(node);
            node = next;
        }
    }

    free(map);
}

// ==========
// iterator

llmap_node_t *llmap_iter_next_(llmap_t *map, llmap_iter_t *iter) {
    if (iter->node && iter->node->next) {
        iter->node = iter->node->next;
    } else {
        llmap_node_t **buckets = llmap_buckets(map);

        do {
            if (iter->idx >= map->cap - 1) {
                iter->idx = map->cap - 1;
                return NULL;
            }
            
            iter->node = buckets[iter->idx];
            iter->idx++;
        } while (!iter->node);
    }
    
    return iter->node;
}
