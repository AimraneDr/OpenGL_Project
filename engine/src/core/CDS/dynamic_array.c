#include "core/CDS/dynamic_array.h"

#include "core/logger.h"
#include "core/CDS/strings.h"
#include <stdlib.h>
#include <string.h>

void* _dynamic_array_create(u64 length, u64 stride) {
    u64 header_size = DARRAY_FIELD_LENGTH * sizeof(u64);
    u64 array_size = length * stride;
    u64* new_array = malloc(header_size + array_size);
    memset(new_array, 0, header_size + array_size);
    new_array[DARRAY_CAPACITY] = length;
    new_array[DARRAY_LENGTH] = 0;
    new_array[DARRAY_STRIDE] = stride;
    return (void*)(new_array + DARRAY_FIELD_LENGTH);
}

void _dynamic_array_destroy(void* array) {
    u64* header = (u64*)array - DARRAY_FIELD_LENGTH;
    u64 header_size = DARRAY_FIELD_LENGTH * sizeof(u64);
    u64 total_size = header_size + header[DARRAY_CAPACITY] * header[DARRAY_STRIDE];
    free(header);
}

u64 _dynamic_array_field_get(void* array, u64 field) {
    u64* header = (u64*)array - DARRAY_FIELD_LENGTH;
    return header[field];
}

void _dynamic_array_field_set(void* array, u64 field, u64 value) {
    u64* header = (u64*)array - DARRAY_FIELD_LENGTH;
    header[field] = value;
}

void* _dynamic_array_resize(void* array) {
    u64 length = dynamic_array_length(array);
    u64 stride = dynamic_array_stride(array);
    void* temp = _dynamic_array_create(
        (DARRAY_RESIZE_FACTOR * dynamic_array_capacity(array)),
        stride);
    memcpy(temp, array, length * stride);

    _dynamic_array_field_set(temp, DARRAY_LENGTH, length);
    _dynamic_array_destroy(array);
    return temp;
}

void* _dynamic_array_push(void* array, const void* value_ptr) {
    u64 length = dynamic_array_length(array);
    u64 stride = dynamic_array_stride(array);
    if (length >= dynamic_array_capacity(array)) {
        array = _dynamic_array_resize(array);
    }

    u64 addr = (u64)array;
    addr += (length * stride);
    memcpy((void*)addr, value_ptr, stride);
    _dynamic_array_field_set(array, DARRAY_LENGTH, length + 1);
    return array;
}

void _dynamic_array_pop(void* array, void* dest) {
    u64 length = dynamic_array_length(array);
    u64 stride = dynamic_array_stride(array);
    u64 addr = (u64)array;
    addr += ((length - 1) * stride);
    memcpy(dest, (void*)addr, stride);
    _dynamic_array_field_set(array, DARRAY_LENGTH, length - 1);
}

void* _dynamic_array_pop_at(void* array, u64 index, void* dest) {
    u64 length = dynamic_array_length(array);
    u64 stride = dynamic_array_stride(array);
    if (index >= length) {
        ERR("Index outside the bounds of this array! Length: %i, index: %index", length, index);
        return array;
    }

    u64 addr = (u64)array;
    memcpy(dest, (void*)(addr + (index * stride)), stride);

    // If not on the last element, snip out the entry and copy the rest inward.
    if (index != length - 1) {
        memcpy(
            (void*)(addr + (index * stride)),
            (void*)(addr + ((index + 1) * stride)),
            stride * (length - index));
    }

    _dynamic_array_field_set(array, DARRAY_LENGTH, length - 1);
    return array;
}

void* _dynamic_array_insert_at(void* array, u64 index, void* value_ptr) {
    u64 length = dynamic_array_length(array);
    u64 stride = dynamic_array_stride(array);
    if (index >= length) {
        ERR("Index outside the bounds of this array! Length: %i, index: %index", length, index);
        return array;
    }
    if (length >= dynamic_array_capacity(array)) {
        array = _dynamic_array_resize(array);
    }

    u64 addr = (u64)array;

    // If not on the last element, copy the rest outward.
    if (index != length - 1) {
        memcpy(
            (void*)(addr + ((index + 1) * stride)),
            (void*)(addr + (index * stride)),
            stride * (length - index));
    }

    // Set the value at the index
    memcpy((void*)(addr + (index * stride)), value_ptr, stride);

    _dynamic_array_field_set(array, DARRAY_LENGTH, length + 1);
    return array;
}