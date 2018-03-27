# Algorithms and Data Structures

Run tests:

```bash
$ cmake . && make && ./ads
-- Configuring done
-- Generating done
-- Build files have been written to: ...
...
```

## Vector (`vec.h`)

Simple dynamic array implementation.

* Works with any data type.
* Grows dynamically.

```c
// any pointer type, must be initialized with NULL
T *vec = NULL;

// push value
int vec_push(T *vec, T); // returns 1 on succes, 0 on fail

// get value at index, unsafe
T vec[0];

// get last value, unsafe
T vec_last(T *vec);

// get vector length, safe
uint64_t vec_len(T *vec);

// get vector capacity, safe
uint64_t vec_cap(T *vec);

// pop last value, unsafe
T vec_pop(T *vec);

// free memory
void vec_free(T *vec); // now vec == NULL
```

## Bit array (`bits.h`)

```c
// allocate 1 million bits
bits_t *bits = bits_new(1000000); // returns NULL on fail

// set bit at index, unsafe
void bits_set(bits_t *bits, uint64_t bit);

// check bit at index, unsafe
int bits_test(bits_t *bits, uint64_t bit); // returns 1 if bit set

// clear bit at index, unsafe
void bits_clear(bits_t *bits, uint64_t bit);

// don't forget to free memory
void bits_free(bits_t *bits);
```

## Todo

- [x] Dynamic array
- [x] Bit array
- [ ] Bloom filter
- [ ] Doubly linked list
- [ ] Hash table using linked lists
- [ ] Hash table using search tree
- [ ] Priority queue using heap
- [ ] Trees & etc...
- [ ] Rabin–Karp algorithm
- [ ] Dijkstra's algorithm
