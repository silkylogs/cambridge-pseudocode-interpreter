#include "lblcont.h"

#include <string.h>
#include <stdlib.h>
#include <assert.h>

struct ProgMem pmnew(cell newcap) {
    struct ProgMem pm;
    memset(&pm, 0, sizeof pm);

    pm.cap = newcap;
    pm.sz = 0;
    pm.memory = calloc(pm.cap, sizeof(uchar));

    memset(pm.memory, 0, pm.cap);

    return pm;
}

void pmresize(
    struct ProgMem *pm,
    cell newcap
) {
    uchar *m = realloc(pm->memory, newcap);
    if (m) {
        pm->memory = m;
        pm->cap = newcap;
    } else {
        assert(!"Resize failed");
    }
}

void pmfree(struct ProgMem *pm) {
    free(pm->memory);
}

void write(
    struct ProgMem *pm,
    void *src,
    cell srcsz
) {
    assert(pm->sz + srcsz <= pm->cap);
    memcpy(pm->memory + pm->sz, src, srcsz);
    pm->sz += srcsz;
}

void writecell(
    struct ProgMem *pm,
    cell c
) {
    assert(pm->sz + sizeof c <= pm->cap);
    write(pm, &c, sizeof c);
}

cell readcell(
    struct ProgMem *pm,
    cell adr
) {
    cell c;
    assert(adr <= pm->cap);
    memcpy(&c, pm->memory + adr, sizeof c);
    return c;
}

void writezstr(
    struct ProgMem *pm,
    char *zstr
) {
    size_t len = strlen(zstr) + 1;
    assert(pm->sz + len <= pm->cap);
    write(pm, zstr, len);
}

// -- forward search
// -- str0 nul ptr str1 nul ptr
// streq()
// if equal
// - padding skip
// - return ptr
// else
// - skip to next str
// - if next str
//   - goto streq
// - else
//   - return null

void writedata(
    struct ProgMem *pm,
    struct Data what
) {
    assert(pm->sz + sizeof what.sz <= pm->cap);
    write(pm, &what.sz, sizeof what.sz);
    assert(pm->sz + what.sz <= pm->cap);
    write(pm, what.data, what.sz);
}

// Round up size to nearest sizeof(cell)
cell pad(cell c) {
    cell next = c + sizeof c;
    cell diff = c % sizeof c;
    return next - diff;
}

void lbl_insert(struct ProgMem *pm,
    char *name,
    cell ptr_to_data
) {
     // Link
    cell head = pm->sz;
    assert(pm->sz + sizeof(cell) <= pm->cap);
    writecell(pm, pm->llhead);
    pm->llhead = head;

    // Len
    cell len = strlen(name) + 1;
    cell padsz = pad(len) - len;
    assert(pm->sz + sizeof(cell) <= pm->cap);
    writecell(pm, len + padsz);

    // Str + padding
    // Str
    assert(pm->sz + strlen(name) + 1 <= pm->cap);
    writezstr(pm, name);

    // Padding
    cell zero = 0;
    assert(pm->sz + padsz <= pm->cap);
    write(pm, &zero, padsz);

    // &data
    assert(pm->sz + sizeof(cell) <= pm->cap);
    writecell(pm, ptr_to_data);
}


// -- reverse pointer
// -- link strpadsz zstr pad ptr

cell adrlink(
    struct ProgMem *pm,
    cell adr
) {
    assert(pm->sz + adr <= pm->cap);

    cell rv = adr + (0 * sizeof adr);
    assert(pm->sz + rv <= pm->cap);

    return rv;
}

cell adrstrpadsz(
    struct ProgMem *pm,
    cell adr
) {
    assert(pm->sz + adr <= pm->cap);

    cell rv = adr + (1 * sizeof adr);
    assert(pm->sz + rv <= pm->cap);

    return rv;
}

cell adrzstr(
    struct ProgMem *pm,
    cell adr
) {
    assert(pm->sz + adr <= pm->cap);

    cell rv = adr + (2 * sizeof adr);
    assert(pm->sz + rv <= pm->cap);

    return rv;
}

cell adrpointer(
    struct ProgMem *pm,
    cell adr
) {
    assert(adr <= pm->cap);

    cell adr_strpadsz = adrstrpadsz(pm, adr);
    assert(adr_strpadsz <= pm->cap);

    cell zstrpadsz = readcell(pm, adr_strpadsz);
    cell adr_pointer = adr + (2 * sizeof adr) + zstrpadsz;

    assert(adr_pointer <= pm->cap);
    return adr_pointer;
}

cell readlink(
    struct ProgMem *pm,
    cell adr
) {
    assert(adr <= pm->cap);

    cell o = adrlink(pm, adr);
    assert(o <= pm->cap);

    return readcell(pm, o);
}

cell readstrpadsz(
    struct ProgMem *pm,
    cell adr
) {
    assert(adr <= pm->cap);

    cell o = adrstrpadsz(pm, adr);
    assert(o <= pm->cap);

    return readcell(pm, o);
}

char *readzstr(
    struct ProgMem *pm,
    cell adr
) {
    assert(adr <= pm->cap);
    return pm->memory + adrzstr(pm, adr);
}

cell readpointer(
    struct ProgMem *pm,
    cell adr
) {
    assert(adr <= pm->cap);

    cell o = adrpointer(pm, adr);
    assert(o <= pm->cap);

    return readcell(pm, o);
}

// ptr = top
// streq(ptr->zstr)
// if equal
// - return ptr
// else
// - if null link return null
// - ptr = *link
cell lbl_find(struct ProgMem *pm,
    char *name
) {
    cell ptr = pm->llhead;
    while (ptr) {
        char *found = readzstr(pm, ptr);
        if (0 == strcmp(name, found)) {
            return readpointer(pm, ptr);
        }
        ptr = readlink(pm, ptr);
    }

    return 0;
}
