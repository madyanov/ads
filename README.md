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
int *vec = NULL;

// push value
vec_push(vec, 100); // returns 1 on succes, 0 on fail

// get value at index, unsafe
vec[0]; // 100

// get last value, unsafe
vec_last(vec); // 100

// get vector length, safe
vec_len(vec); // 1

// get vector capacity, safe
vec_cap(vec); // 1

// pop last value, unsafe
vec_pop(vec); // 100

// free memory
vec_free(vec); // now vec == NULL
```

## Bit array (`bits.h`)

```c
// allocate 1 million bits
bits_t *bits = bits_new(100000); // returns 1 on success, 0 on fail

// set bit at index 3, safe
bits_set(bits, 3); // returns 1 on success, 0 on fail

// check bit at index 3
bits_test(bits, 3); // 1, returns 1 if bit sat

// clear bit at index 3
bits_clear(bits, 3); // returns 1 on success, 0 on fail

bits_test(bits, 3); // 0

// don't forget to free memory
bits_free(bits);
```
