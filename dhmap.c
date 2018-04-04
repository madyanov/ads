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
    node->state = filled;
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

dhmap_node_t *dhmap_node(dhmap_t *map, const char *key, size_t tsize) {
    uint32_t hash = dhmap_hash1(key);
    size_t idx = dhmap_mod(map, hash);
    size_t step = dhmap_mod(map, dhmap_hash2(key));
    dhmap_node_t *node = NULL;

    for (;;) {
        node = dhmap_node_at(map, idx, tsize);

        if (node->state == empty) {
            return node;
        }

        if (node->hash == hash && strncmp(key, node->key, dhmap_key_len) == 0) {
            return node;
        }

        idx = dhmap_mod(map, idx + step);
    }

    return NULL;
}

int dhmap_set_(dhmap_t **map, const char *key, void *val, size_t tsize) {
    uint32_t hash = dhmap_hash1(key);
    dhmap_node_t *node = dhmap_node(*map, key, tsize);

    if (node->state != filled) {
        (*map)->len++;
    }

    dhmap_node_set(node, hash, key, val, tsize);
    return 1;
}

void *dhmap_get_(dhmap_t *map, const char *key, size_t tsize) {
    dhmap_node_t *node = dhmap_node(map, key, tsize);

    if (node && node->state == filled) {
        return node->val;
    }
    
    return NULL;
}

int dhmap_del_(dhmap_t **map, const char *key, size_t tsize) {
    dhmap_node_t *node = dhmap_node(*map, key, tsize);
    
    if (node && node->state == filled) {
        node->state = deleted;
        (*map)->len--;
    }

    return 1;
}
