# Algorithms and Data Structures

* Functions marked as `unsafe` and called with invalid arguments may result UB, segfault, etc.
* Run tests:

    ```bash
    $ cmake . && make && ./ads
    ...
    OK.
    ```

## Contents

* [Vector](#vector-vech)
* [Bit map](#bit-map-bitsh)
* [Rabin-Karp algorithm](#rabin-karp-string-searching-algorithm-rkh)
* [Bloom filter](#bloom-filter-bloomh)
* [Todo](#todo)

## Vector (`vec.h`)

Simple dynamic array implementation.

* Works with any data type.
* Grows dynamically.

```c
// any pointer type, must be initialized with NULL
T *vec = NULL;

// push value, safe
// value will have index `vec_len - 1`
int vec_push(T *vec, T val); // return 1 on success, 0 on fail

// push value, all values up to `vec_cap` will be initialized with 0
int vec_push_zero(T *vec, T val);

// get value at index, unsafe
T vec[0];

// get last value, unsafe
T vec_last(T *vec);

// get vector length, safe
// returns index of the last pushed value
size_t vec_len(T *vec);

// get vector capacity, safe
// you can safely access and change any value up to `vec_cap`
size_t vec_cap(T *vec);

// pop last value, unsafe
T vec_pop(T *vec);

// clear vector, safe
// just sets vector length to 0
void vec_clear(T *vec);

// free memory, safe
void vec_free(T *vec); // now vec == NULL
```

```c
char *str = NULL;

vec_push(str, 'H');
vec_push(str, 'e');
vec_push(str, 'l');
vec_push(str, 'l');
vec_push(str, 'o');
vec_push(str, '\0');

assert(strcmp(str, "Hello") == 0);

vec_free(str);
```

## Bit map (`bits.h`)

Simple bit map **without bounds checking**.

```c
// allocate 1 million bits
bits_t *bits = bits_new(1000000); // will return NULL if allocation failed

// set bit at index, unsafe
void bits_set(bits_t *bits, size_t bit);

// test bit at index, unsafe
int bits_test(bits_t *bits, size_t bit); // return 1 if bit set

// clear bit at index, unsafe
void bits_clear(bits_t *bits, size_t bit);

// free memory, safe
void bits_free(bits_t *bits);
```

## Rabin-Karp string searching algorithm (`rk.h`)

Requires `vec.h`.

```c
// search occurrences of pattern in text
size_t *rk_search(const char *text, const char *patt); // will return NULL if arguments are invalid
```

```c
size_t *occs = rk_search("XZX", "X");
vec_len(occs); // 2
occs[0]; // 0
occs[1]; // 2
```

## Bloom filter (`bloom.h`)

Requires `bits.h`, `murmur3.h`.

```c
// create bloom filter to store `count` elements
// with false positive probability of `fpp`
bloom_t *bloom_new(size_t count, float fpp);

// add element `key` with length `len` to bloom filter, unsafe
void bloom_add(bloom_t *bloom, const void *key, size_t len);

// check if element `key` with length `len` exists in bloom filter, unsafe
int bloom_has(bloom_t *bloom, const void *key, size_t len); // return 0 if NOT exists, 1 if EXISTS or NOT EXISTS

// free memory, safe
void bloom_free(bloom_t *bloom);
```

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
- [ ] Doubly linked list
- [ ] Hash table using linked lists
- [ ] Hash table using search tree
- [ ] Priority queue using heap
- [ ] Trees & etc...
- [ ] Dijkstra's algorithm
