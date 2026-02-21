#ifndef LABEL_CONTAINER_H
#define LABEL_CONTAINER_H

#include <stdint.h>

typedef unsigned char uchar;
typedef uint32_t cell;

struct ProgMem {
    uchar *memory;
    cell sz;
    cell cap;
    cell llhead; // Head of linked list
};

struct ProgMem pmnew(cell newcap);
void pmresize(struct ProgMem *pm, cell newcap);
void pmfree(struct ProgMem *pm);

void write(struct ProgMem *pm, void *src, cell srcsz);
void writecell(struct ProgMem *pm, cell c);
cell readcell(struct ProgMem *pm, cell adr);

void writezstr(struct ProgMem *pm, char *zstr);


// -- data container
// -- padsz data pad
struct Data {
    cell sz; // Data + padding, aligned to cell
    uchar *data;
};
void writedata(struct ProgMem *pm, struct Data what);

// Round up size to nearest sizeof(cell)
cell pad(cell c);

void insert(struct ProgMem *pm, char *name, cell ptr_to_data);


// -- reverse pointer
// -- link strpadsz zstr pad ptr

cell adrlink(struct ProgMem *pm, cell adr);
cell adrstrpadsz(struct ProgMem *pm, cell adr);
cell adrzstr(struct ProgMem *pm, cell adr);
cell adrpointer(struct ProgMem *pm, cell adr);

cell readlink(struct ProgMem *pm, cell adr);
cell readstrpadsz(struct ProgMem *pm, cell adr);
char *readzstr(struct ProgMem *pm, cell adr);
cell readpointer(struct ProgMem *pm, cell adr);

intptr_t find(struct ProgMem *pm, char *name);

#endif
