# Algorithms and Data Structures

Run tests:

```bash
$ cmake . && make && ./ads
-- Configuring done
...
```

## Vector (`vec.h`)

Simple dynamic array implementation.

* Works with any data type.
* Grows dynamically.

```c
// any pointer type, must be initialized with NULL
T *vec = NULL;

// push value, safe
int vec_push(T *vec, T); // return 1 on success, 0 on fail

// get value at index, unsafe
T vec[0];

// get last value, unsafe
T vec_last(T *vec);

// get vector length, safe
size_t vec_len(T *vec);

// get vector capacity, safe
size_t vec_cap(T *vec);

// pop last value, unsafe
T vec_pop(T *vec);

// clear vector, safe
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

```c
// allocate 1 million bits
bits_t *bits = bits_new(1000000); // will return NULL if allocation failed

// set bit at index, unsafe
void bits_set(bits_t *bits, size_t bit);

// check bit at index, unsafe
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
void rk_search(size_t **occs, const char *text, const char *patt);
```

```c
size_t *occs = NULL; // vec.h
rk_search(&occs, "XZX", "X");
occs[0]; // 0
occs[1]; // 2
```

## Todo

- [x] Dynamic array
- [x] Bit array
- [x] Rabin–Karp algorithm
- [ ] Bloom filter
- [ ] Doubly linked list
- [ ] Hash table using linked lists
- [ ] Hash table using search tree
- [ ] Priority queue using heap
- [ ] Trees & etc...
- [ ] Dijkstra's algorithm
