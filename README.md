# Algorithms and Data Structures

Run tests:

    $ cmake . && make && ./ads
    -- Configuring done
    -- Generating done
    -- Build files have been written to: ...
    ...

## Vector (`vec.h`)

Simple dynamic array implementation.

* Works with any data type.
* Grows dynamically.

### Usage

    // any pointer type, must be initialized with NULL
    int *vec = NULL;

    // push value
    vec_push(vec, 100); // returns 1 on succes, otherwise 0

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
