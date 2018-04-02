# Algorithms and Data Structures

Run:

```bash
$ git clone git@github.com:madyanov/ads.git
$ cmake . && make && ./ads
...
```

## Contents

* [Vector](#vector-vech)
* [Linked list based hash map](#linked-list-based-hash-map-llmaph)
* [Bit map](#bit-map-bitsh)
* [Rabin-Karp algorithm](#rabin-karp-string-searching-algorithm-rkh)
* [Bloom filter](#bloom-filter-bloomh)
* [Todo](#todo)

## Vector (`vec.h`)

Simple dynamic array implementation.

* Works with any data type.
* Grows/shrinks dynamically.
* Value semantics.

Layout:

```
    cap = 8
    len = 4
    ___ = *
          |
          v
          [ occupied | occupied | occupied | occupied | garbage | garbage | garbage | garbage ]
```

API:

```c
// any pointer type
T *vec;

// initialize vector
int vec_init(vec); // return 1 on success, 0 on fail

// push value
int vec_push(T *vec, T val); // return 1 on success, 0 on fail

// get value at index
T vec[0];

// get last value
T vec_last(T *vec);

// get vector length
size_t vec_len(T *vec);

// pop last value
T vec_pop(T *vec);

// clear vector
void vec_clear(T *vec);

// free memory
void vec_free(T *vec);
```

Example:

```c
char *str;
vec_init(str);

vec_push(str, 'H');
vec_push(str, 'e');
vec_push(str, 'l');
vec_push(str, 'l');
vec_push(str, 'o');
vec_push(str, '\0');

assert(strcmp(str, "Hello") == 0);

vec_free(str);
```

## Linked list based hash map (`llmap.h`)

* Works with any data type.
* Grows/shrinks dynamically.
* Value semantics.

Layout:

```
    cap     = 8
    len     = 4
    buckets = *
              |
              v
              [ empty | * occupied | empty | empty | * occupied | empty | empty | empty ]
                        |                            |
                        v                            v
                        [ hash = .                   [ hash = .
                          key  = .                     key  = .
                          data = .                     data = .
                          next = * ]                   next = * ]
                                 |                            |
                                 v                            v
                                 [ hash = .                   [ hash = .
                                   key  = .                     key  = .
                                   data = .                     data = .
                                   next = NULL ]                next = NULL ]
```

API:

```c
// define map with elements of type T
llmap_t(T) map;

// initialize map
void *llmap_init(llmap_t(T) map); // will return NULL if failed

// set value to the null-terminated key
int llmap_set(llmap_t(T) map, const char *key, void *val); // will return 0 if failed

// get *pointer* to value in the map
T *llmap_get(llmap_t(T) map, const char *key); // will return NULL if key not exists

// delete element at key
void llmap_del(llmap_t(T) map, const char *key);

// get map length (count of the elements)
llmap_len(llmap_t(T) map);

// free memory
llmap_free(llmap_t(T) map);

// initialize or reset iterator
void llmap_iter_init(llmap_t(T) map);

// get next map node
llmap_node_t *llmap_iter_next(llmap_t(T) map); // will return NULL if no more elements available

// get node key
node->key; // char *

// get *pointer* to node value
T *llmap_node_val(llmap_t(T) map, llmap_node_t *node);
```

Example:

```c
llmap_t(int) imap;
llmap_init(imap);

llmap_set(imap, "key1", 100);
*llmap_get(imap, "key1"); // 100

llmap_del(imap, "key1");
llmap_get(imap, "key1"); // NULL

// iterator
llmap_iter_init(imap);
llmap_node_t *node;

while ((node = llmap_iter_next(imap))) {
    char *key = node->key;
    int val = *llmap_node_val(imap, node);
}

llmap_free(imap);
```

## Bit map (`bits.h`)

Simple bit map **without bounds checking**.

API:

```c
// allocate 1 million zero bits
bits_t *bits = bits_new(1000000); // will return NULL if allocation failed

// set bit at index
void bits_set(bits_t *bits, size_t idx);

// test bit at index
int bits_test(bits_t *bits, size_t idx); // return 1 if bit set

// clear bit at index
void bits_clear(bits_t *bits, size_t idx);

// free memory
void bits_free(bits_t *bits);
```

## Rabin-Karp string searching algorithm (`rk.h`)

Requires `vec.h`.

API:

```c
// search occurrences of pattern in text, occs must be initialized (`vec_init`)
void rk_search(size_t **occs, const char *text, const char *patt);
```

Example:

```c
size_t *occs;
vec_init(occs);

rk_search(&occs, "XZZX", "X");
vec_len(occs); // 2
occs[0]; // 0
occs[1]; // 3

vec_free(occs);
```

## Bloom filter (`bloom.h`)

Requires `bits.h`, `murmur3.h`.

API:

```c
// create bloom filter to store `cap` elements with false positive probability `fpp`
bloom_t *bloom_new(size_t cap, float fpp);

// add element
void bloom_add(bloom_t *bloom, const void *key, size_t len);

// check if element exists
int bloom_has(bloom_t *bloom, const void *key, size_t len); // return 0 if NOT exists, 1 if EXISTS or NOT EXISTS

// free memory
void bloom_free(bloom_t *bloom);
```

Example:

```c
bloom_t *bloom = bloom_new(10, 0.1);

bloom_add(bloom, "a", 1);
bloom_has(bloom, "a", 1); // 1

bloom_add(bloom, "c", 1);
bloom_has(bloom, "c", 1); // 1

bloom_has(bloom, "b", 1); // 0

bloom_free(bloom);
```

## Todo

- [x] Dynamic array
- [x] Bit array
- [x] Rabin–Karp algorithm
- [x] Bloom filter
- [x] Hash table using linked lists
- [ ] Hash table using linear probing
- [ ] Hash table using search tree
- [ ] Priority queue using heap
- [ ] Dijkstra's algorithm
