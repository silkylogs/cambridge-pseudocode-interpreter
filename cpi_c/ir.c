#include "ir.h"

#include <assert.h>
#include <stdlib.h>

#define TODO assert(!"Todo");

static void grow(unsigned char **backing_memory, size_t new_sz) {
    void *ptr = realloc(*backing_memory, new_sz);
    if (!ptr) assert(!"Realloc failure"); // For now.
    memset(ptr, 0, new_sz);
    *backing_memory = ptr;
}

// Note: `sz` may be off by +1
static size_t padded_size(size_t sz, size_t alignment) {
    sz -= 1;
    return sz + alignment - (sz % alignment);
}

void data_container_add(
    unsigned char *backing_memory,
    size_t *inout_backing_memory_size,
    unsigned char *data,
    size_t data_size_bytes
) {
    size_t old_backing_memory_size = *inout_backing_memory_size;
    size_t new_sz = *inout_backing_memory_size + data_size_bytes;
    new_sz = padded_size(new_sz, sizeof(intptr_t));

    *inout_backing_memory_size = new_sz;
    grow(&backing_memory, *inout_backing_memory_size);

    unsigned char *one_past_end_of_first = backing_memory + old_backing_memory_size;
    memcpy(one_past_end_of_first, data, data_size_bytes);
}

void label_container_add(
    unsigned char *backing_memory,
    size_t *inout_backing_memory_size,
    intptr_t *inout_based_ptr_latest,
    char *zstr_name,
    intptr_t based_ptr_data
) {
    size_t old_backing_memory_size = *inout_backing_memory_size;
    size_t link_sz = sizeof(intptr_t);
    size_t zstrpadsz_sz = sizeof(intptr_t);
    size_t zstrpad_sz = padded_size(strlen(zstr_name) + 1, sizeof(intptr_t));
    size_t ptr_sz = sizeof(intptr_t);

    size_t new_sz = old_backing_memory_size + link_sz + zstrpadsz_sz + zstrpad_sz + ptr_sz;
    *inout_backing_memory_size = new_sz;
    grow(&backing_memory, *inout_backing_memory_size);

    unsigned char *base = backing_memory + old_backing_memory_size;

    *(intptr_t *)(base + 0 * sizeof(intptr_t)) = *inout_based_ptr_latest;
    *inout_based_ptr_latest = base - backing_memory;

    *(intptr_t *)(base + 1 * sizeof(intptr_t)) = zstrpad_sz;

    memset(base + 2 * sizeof(intptr_t), 0, zstrpad_sz);
    strcpy(base + 2 * sizeof(intptr_t), zstr_name);

    *(intptr_t *)(base + link_sz + zstrpadsz_sz + zstrpad_sz) = based_ptr_data;
}

intptr_t label_container_find(
    unsigned char *backing_memory,
    intptr_t based_ptr_latest,
    char *search_term
) {
    intptr_t ptr = based_ptr_latest;
    size_t link_sz = sizeof(intptr_t);
    size_t zstrpadsz_sz = sizeof(intptr_t);
    char *comparee;
loop:
    comparee = backing_memory + ptr + link_sz + zstrpadsz_sz;
    if (0 == strcmp(comparee, search_term)) {
        size_t zstrpad_sz = backing_memory + ptr + link_sz;
        return *(intptr_t *)(backing_memory + link_sz + zstrpadsz_sz + zstrpad_sz);
    }

    // Not found
    if (*(intptr_t*)(backing_memory + ptr)) {
        ptr = *(intptr_t*)(backing_memory + ptr);
        goto loop;
    } else {
        return 0;
    }
}