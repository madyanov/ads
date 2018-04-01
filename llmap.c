#include "llmap.h"

// ==========
// nodes

uint32_t llmap_hash(const char *key) {
    uint32_t hash = 5381;

    while (*key) {
        hash = ((hash << 5) + hash) ^ *key++;
    }

    return hash;
}

void *llmap_node_val_(llmap_node_t *node, size_t klen) {
    return node->key + (klen ? klen : (strlen(node->key) + 1));
}

llmap_node_t *llmap_node_new(const char *key, void *val, size_t vsize) {
    size_t klen = strlen(key) + 1;
    llmap_node_t *node = malloc(offsetof(llmap_node_t, key) + klen + vsize);

    if (!node) {
        return NULL;
    }

    memcpy(node->key, key, klen);
    memcpy(llmap_node_val_(node, klen), val, vsize);

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
    llmap_t *map = calloc(1, offsetof(llmap_t, buckets) + sizeof(llmap_node_t *) * llmap_init_cap);

    if (!map) {
        return NULL;
    }

    map->cap = llmap_init_cap;
    map->len = 0;
    return map;
}

size_t llmap_idx(llmap_t *map, uint32_t hash) {
    return hash & (map->cap - 1);
}

llmap_node_t **llmap_get_node(llmap_t *map, const char *key) {
    uint32_t hash = llmap_hash(key);
    llmap_node_t **node = &map->buckets[llmap_idx(map, hash)];

    while (*node) {
        if ((*node)->hash == hash && strcmp(key, (*node)->key) == 0) {
            return node;
        }

        node = &(*node)->next;
    }

    return NULL;
}

void llmap_add_node(llmap_t *map, llmap_node_t *node) {
    size_t idx = llmap_idx(map, node->hash);
    node->next = map->buckets[idx];
    map->buckets[idx] = node;
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

    llmap_node_t *node, *next, *first = NULL;

    for (size_t i = 0; i < map->cap; i++) {
        node = map->buckets[i];

        while (node) {
            next = node->next;
            node->next = first;
            first = node;
            node = next;
        }
    }

    llmap_t *new_map = realloc(map, offsetof(llmap_t, buckets) + sizeof(llmap_node_t *) * cap);

    if (!new_map) {
        return NULL;
    }

    memset(new_map->buckets, 0, sizeof(llmap_node_t *) * cap);
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
    llmap_node_t *node, *next = NULL;

    for (size_t i = 0; i < map->cap; i++) {
        node = map->buckets[i];

        while (node) {
            next = node->next;
            llmap_node_free(node);
            node = next;
        }
    }

    free(map);
}

void llmap_print_distr_(llmap_t *map) {
    llmap_node_t *next = NULL;
    size_t count, collisions = 0;

    printf("bucket\tcount\n");

    for (size_t i = 0; i < map->cap; i++) {
        next = map->buckets[i];
        count = 0;

        while (next) {
            next = next->next;
            count++;
        }

        if (count > 1) {
            collisions++;
        }

        printf("%zu\t%zu\n", i, count);
    }

    printf("collisions %zu\n", collisions);
}

// ==========
// iterator

llmap_node_t *llmap_iter_next_(llmap_t *map, llmap_iter_t *iter) {
    if (iter->node && iter->node->next) {
        iter->node = iter->node->next;
    } else {
        do {
            if (iter->idx >= map->cap) {
                iter->idx = map->cap;
                return NULL;
            }
            
            iter->node = map->buckets[iter->idx];
            iter->idx++;
        } while (!iter->node);
    }
    
    return iter->node;
}
