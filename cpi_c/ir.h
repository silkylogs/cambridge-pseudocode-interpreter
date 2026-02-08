#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#ifndef IR_H
#define IR_H

// -- Data container --
// Format: datasz data pad
// - Size of data, in bytes.
// - Data.
// - Optional padding.

void data_container_add(
    unsigned char *backing_memory,
    size_t *inout_backing_memory_size,
    unsigned char *data,
    size_t data_size_bytes);

// -- Label container --
// Cell size: intptr_t
// Format: Link zstrpadsz zstr pad ptr -> (Data container)
// - Link pointer. Is a null terminated linked list.
// - Number of bytes needed to contain null terminated string with padding.
// - Null terminated string.
// - Optional padding.
// - Pointer based from backing memory, intptr_t aligned.

void label_container_add(
    unsigned char *backing_memory,
    size_t *inout_backing_memory_size,
    intptr_t *inout_based_ptr_latest,
    char *zstr_name,
    intptr_t based_ptr_data);

intptr_t label_container_find(
    unsigned char *backing_memory,
    intptr_t based_ptr_latest,
    char *search_term);

#endif
