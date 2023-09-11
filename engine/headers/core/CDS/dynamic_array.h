#pragma once

#include "defines.h"

/*
Memory layout
u64 capacity = number elements that can be held
u64 length = number of elements currently contained
u64 stride = size of each element in bytes
void* elements
*/

enum {
    DARRAY_CAPACITY,
    DARRAY_LENGTH,
    DARRAY_STRIDE,
    DARRAY_FIELD_LENGTH
};

DLL_EXPORT void* _dynamic_array_create(u64 length, u64 stride);
DLL_EXPORT void _dynamic_array_destroy(void* array);

DLL_EXPORT u64 _dynamic_array_field_get(void* array, u64 field);
DLL_EXPORT void _dynamic_array_field_set(void* array, u64 field, u64 value);

DLL_EXPORT void* _dynamic_array_resize(void* array);

DLL_EXPORT void* _dynamic_array_push(void* array, const void* value_ptr);
DLL_EXPORT void _dynamic_array_pop(void* array, void* dest);

DLL_EXPORT void* _dynamic_array_pop_at(void* array, u64 index, void* dest);
DLL_EXPORT void* _dynamic_array_insert_at(void* array, u64 index, void* value_ptr);

#define DARRAY_DEFAULT_CAPACITY 1
#define DARRAY_RESIZE_FACTOR 2

#define dynamic_array_create(type) \
    (type *)_dynamic_array_create(DARRAY_DEFAULT_CAPACITY, sizeof(type))

#define dynamic_array_reserve(type, capacity) \
    _dynamic_array_create(capacity, sizeof(type))

// #define dynamic_array_free_elems(array) _dynamic_array_free_elems(array);

#define dynamic_array_destroy(array) _dynamic_array_destroy(array);

#define dynamic_array_field_get(buffer, index) _dynamic_array_field_get(buffer,index);

#define dynamic_array_field_set(buffer, index, value) _dynamic_array_field_set(buffer,index, value);

#define dynamic_array_push(array, value)           \
    {                                       \
        typeof(value) temp = value;         \
        array = _dynamic_array_push(array, &temp); \
    }
// NOTE: could use __auto_type for temp above, but intellisense
// for VSCode flags it as an unknown type. typeof() seems to
// work just fine, though. Both are GNU extensions.

#define dynamic_array_pop(array, value_ptr) \
    _dynamic_array_pop(array, value_ptr)

#define dynamic_array_insert_at(array, index, value)           \
    {                                                   \
        typeof(value) temp = value;                     \
        array = _dynamic_array_insert_at(array, index, &temp); \
    }

#define dynamic_array_pop_at(array, index, value_ptr) \
    _dynamic_array_pop_at(array, index, value_ptr)

#define dynamic_array_clear(array) \
    _dynamic_array_field_set(array, DARRAY_LENGTH, 0)

#define dynamic_array_capacity(array) \
    _dynamic_array_field_get(array, DARRAY_CAPACITY)

#define dynamic_array_length(array) \
    _dynamic_array_field_get(array, DARRAY_LENGTH)

#define dynamic_array_stride(array) \
    _dynamic_array_field_get(array, DARRAY_STRIDE)

#define dynamic_array_length_set(array, value) \
    _dynamic_array_field_set(array, DARRAY_LENGTH, value)
    