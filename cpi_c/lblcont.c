#include "lblcont.h"

#include <string.h>
#include <stdlib.h>

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
    } else {}
}

void pmfree(struct ProgMem *pm) {
    free(pm->memory);
}

void write(
    struct ProgMem *pm,
    uchar *src,
    cell srcsz
) {
    memcpy(pm->memory + pm->sz, src, srcsz);
    pm->sz += srcsz;
}

void writecell(
    struct ProgMem *pm,
    cell c
) {
    write(pm, &c, sizeof c);
}

cell readcell(
    struct ProgMem *pm,
    cell adr
) {
    cell c;
    memcpy(&c, pm->memory + adr, sizeof c);
    return c;
}

void writezstr(
    struct ProgMem *pm,
    char *zstr
) {
    size_t len = strlen(zstr) + 1;
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
    write(pm, &what.sz, sizeof what.sz);
    write(pm, what.data, what.sz);
}

// Round up size to nearest sizeof(cell)
cell pad(cell c) {
    cell next = c + sizeof c;
    cell diff = c % sizeof c;
    return next - diff;
}

void insert(
    struct ProgMem *pm,
    char *name,
    cell ptr_to_data
) {
    // Link
    cell head = pm->sz;
    writecell(pm, pm->llhead);
    pm->llhead = head;

    // Len
    cell len = strlen(name) + 1;
    cell padsz = pad(len) - len;
    writecell(pm, len + padsz);

    // Str + padding
    writezstr(pm, name);
    cell zero = 0;
    write(pm, &zero, padsz);

    // &data
    writecell(pm, ptr_to_data);
}


// -- reverse pointer
// -- link strpadsz zstr pad ptr

cell adrlink(cell adr) {
    return adr + (0 * sizeof adr);
}

cell adrstrpadsz(cell adr) {
    return adr + (1 * sizeof adr);
}

cell adrzstr(cell adr) {
    return adr + (2 * sizeof adr);
}

cell adrpointer(
    struct ProgMem *pm, 
    cell adr
) {
    cell zstrpadsz = readcell(pm, adrstrpadsz(adr));
    return adr + (2 * sizeof adr) + zstrpadsz;
}

cell readlink(
    struct ProgMem *pm,
    cell adr
) {
    uchar *o = pm->memory + adrlink(adr);
    return readcell(pm, o);
}

cell readstrpadsz(
    struct ProgMem *pm,
    cell adr
) {
    uchar *o = pm->memory + adrstrpadsz(adr);
    return readcell(pm, o);
}

char *readzstr(
    struct ProgMem *pm,
    cell adr
) {
    return pm->memory + adrzstr(adr);
}

cell readpointer(
    struct ProgMem *pm,
    cell adr
) {
    return readcell(pm, adrpointer(pm, adr));
}

// ptr = top
// streq(ptr->zstr)
// if equal
// - return ptr
// else
// - if null link return null
// - ptr = *link
intptr_t find(
    struct ProgMem *pm,
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
