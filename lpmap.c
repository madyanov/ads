#include "lpmap.h"

uint32_t lpmap_hash(const char *key) {
    uint32_t hash = 5381;

    while (*key) {
        hash = ((hash << 5) + hash) ^ *key++;
    }

    return hash;
}

// ==========
// nodes

int lpmap_node_empty(lpmap_node_t *node) {
    return !node->tsize;
}

int lpmap_node_deleted(lpmap_node_t *node) {
    return !node->key[0];
}

int lpmap_node_free(lpmap_node_t *node) {
    return !node->key[0] || !node->tsize;
}

// ==========
// map

lpmap_t *lpmap_new(size_t tsize) {
    lpmap_t *map = calloc(1, offsetof(lpmap_t, buckets) + (sizeof(lpmap_node_t) + tsize) * lpmap_init_cap);

    if (!map) {
        return NULL;
    }

    map->cap = lpmap_init_cap;
    map->len = 0;
    return map;
}

size_t lpmap_idx(lpmap_t *map, uint32_t hash) {
    return hash & (map->cap - 1);
}

size_t lpmap_probs(lpmap_t *map, lpmap_node_t *node, size_t idx) {
    return idx - lpmap_idx(map, node->hash);
}

lpmap_node_t *lpmap_node_at(lpmap_t *map, size_t idx, size_t tsize) {
    return (lpmap_node_t *)((char *)(map->buckets + idx) + idx * tsize);
}

lpmap_node_t *lpmap_node(lpmap_t *map, const char *key, size_t tsize, int deleted) {
    size_t hash = lpmap_hash(key);
    size_t idx = lpmap_idx(map, hash);
    size_t klen = strlen(key);

    lpmap_node_t *node = NULL;

    for (; idx < map->cap; idx++) {
        node = lpmap_node_at(map, idx, tsize);

        if (lpmap_node_empty(node)) {
            return node;
        }

        if (deleted && lpmap_node_deleted(node)) {
            return node;
        }

        if (!lpmap_node_deleted(node) && node->hash == hash && strncmp(key, node->key, lpmap_key_len) == 0) {
            return node;
        }
    }

    return NULL;
}

int lpmap_set_(lpmap_t **map, const char *key, void *val, size_t tsize) {
    lpmap_node_t *node = lpmap_node(*map, key, tsize, 1);

    if (!node) {
        return 0;
    }

    if (!lpmap_node_free(node)) {
        memcpy(node->val, val, tsize);
        return 1;
    }

    node->hash = lpmap_hash(key);
    node->tsize = tsize;
    memcpy(node->key, key, lpmap_key_len);
    memcpy(node->val, val, tsize);
    (*map)->len++;
    return 1;
}

void *lpmap_get_(lpmap_t *map, const char *key, size_t tsize) {
    lpmap_node_t *node = lpmap_node(map, key, tsize, 0);

    if (!node || lpmap_node_free(node)) {
        return NULL;
    }
    
    return node->val;
}

int lpmap_del_(lpmap_t **map, const char *key, size_t tsize) {
    lpmap_node(*map, key, tsize, 0)->key[0] = '\0';
    (*map)->len--;
    return 1;
}

// void lpmap_print_distr_(lpmap_t *map, size_t tsize) {
//     size_t count, collisions = 0;

//     printf("bucket\tdeleted\tfilled\tprobs\n");

//     for (size_t i = 0; i < map->cap; i++) {
//         lpmap_node_t *node = lpmap_node_at(map, i, tsize);
//     }
// }
