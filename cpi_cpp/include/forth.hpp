#pragma once

#include <stdio.h>
#include <stdint.h>
#include <utility>
#include <array>
#include <string>
#include <vector>
#include <iostream>

using Cell = uint16_t;

struct ForthState {
	std::vector<Cell> memory;
	bool exit_requested;

	Cell *backing_memory();
	void overwrite_memory(char *const byte_string, size_t strlen);
	void run();
};

#if 0
// TODO: 

using Cell = uint16_t;
constexpr int mem_cell_count = 0x100;
consteval size_t mem_byte_count(size_t s) { return s * sizeof Cell; }

constexpr char rom[mem_byte_count(mem_cell_count)] =
"\x3D\x00""\x0F\x00""\x17\x00""\x00\x00""\x00\x00""\x00\x00""\x00\x00""\x00\x00" // 0
"\x00\x00""\x00\x00""\x00\x00""\x00\x00""\x00\x00""\x00\x00""\x00\x00""\x00\x00"

"\xCC\xCC""\xCC\xCC""\xCC\xCC""\xCC\xCC""\xCC\xCC""\xCC\xCC""\xCC\xCC""\xCC\xCC" // 10
"\xCC\xCC""\xCC\xCC""\xCC\xCC""\xCC\xCC""\xCC\xCC""\xCC\xCC""\xCC\xCC""\xCC\xCC"

"\x00\x00""\x02\x80""exit"              "\x20\x00""\x00\x00""\x00\x00""\x00\x00" // 20
"\x20\x00""\x02\x80""pchr"              "\x28\x00""\x07\x00""\x00\x00""\x00\x00"

"\x28\x00""\x02\x80""ltrl"              "\x30\x00""\x06\x00""\x00\x00""\x00\x00" // 30
"\x30\x00""\x02\x00""main"              "\x38\x00"
"\x34\x00""\x48\x00""\x2C\x00"
"\x34\x00""\x65\x00""\x2C\x00"
"\x34\x00""\x6C\x00""\x2C\x00"
"\x34\x00""\x6C\x00""\x2C\x00"
"\x34\x00""\x6F\x00""\x2C\x00"
"\x34\x00""\x2C\x00""\x2C\x00"
"\x34\x00""\x20\x00""\x2C\x00"
"\x34\x00""\x77\x00""\x2C\x00"
"\x34\x00""\x6F\x00""\x2C\x00"
"\x34\x00""\x72\x00""\x2C\x00"
"\x34\x00""\x6C\x00""\x2C\x00"
"\x34\x00""\x64\x00""\x2C\x00"  
"\x34\x00""\x21\x00""\x2C\x00"
"\x34\x00""\x0A\x00""\x2C\x00"
"\x24\x00""\x24\x00";

Cell ram[0x100];
Cell &inst_ptr = ram[0x0000];
Cell &dstk_ptr = ram[0x0001];
Cell &rstk_ptr = ram[0x0002];
bool exit_requested = false;

// Primitives

// Note: push semantic: stack pointer always points to the top of the stack.

void exit(Cell *m) { printf("Exiting...\n"); exit_requested = true; };
void nop(Cell* m) { (void)m; printf("nop ");  };
void dpush(Cell *m) { dstk_ptr++; }
void dpop(Cell *m) { dstk_ptr--; }
void rpush(Cell *m) { rstk_ptr++; }
void rpop(Cell *m) { rstk_ptr--; }
void lit(Cell *m) {
    dpush(m);
    m[dstk_ptr] = m[m[rstk_ptr]];
    m[rstk_ptr]++;
}
void printchar(Cell *m) { printf("%c", m[dstk_ptr]); dstk_ptr--; }
void brz(Cell *m) {
    Cell addr = m[dstk_ptr];
    dstk_ptr--;
    Cell cond = m[dstk_ptr];
    dstk_ptr--;
    if (!cond) {
        m[rstk_ptr] = addr;
    }
}
void sto(Cell *m) {
    Cell addr = m[dstk_ptr];
    dstk_ptr--;
    Cell val = m[dstk_ptr];
    dstk_ptr--;
    m[addr] = val;
}
void lod_cell(Cell *m) {
    Cell addr = m[dstk_ptr];
    m[dstk_ptr] = m[addr];
}
void inc(Cell *m) { m[dstk_ptr]++; }

// Helper functions

// Forth word entry: [Link ptr][prim=0,immed,len...][name][header ptr][def]...
// Primitive entry:  [Link ptr][prim=1,immed,len...][name][header ptr][primitive index]
bool entry_is_prim(Cell *m) {
    //return *(start_of_entry + 1) & 0x7000;
    return m[inst_ptr + 1] & 0x8000;
}
bool entry_is_immed(Cell *m) {
    return m[inst_ptr + 1] & 0x4000;
}
Cell entry_strlen(Cell *m) {
    return m[inst_ptr + 1] & 0x3FFF;
}
Cell entry_headerptr(Cell *m) {
    return m[inst_ptr + entry_strlen(m) + 2];
}
Cell entry_1stdefn(Cell *m) {
    return m[inst_ptr + entry_strlen(m) + 3];
}
Cell entry_primidx(Cell *m) { 
    return m[inst_ptr + entry_strlen(m) + 3];
}

Cell entry_linkptr(Cell *m) {
    return m[inst_ptr];
}

// Also doubles as instruction set... technically
std::array<void(*)(Cell*), 12> primitives {
    exit, nop, dpush, dpop, 
    rpush, rpop, lit, printchar, 
    brz, sto, lod_cell, inc, 
};

/* Usage code goes here
int main() {
    memcpy(ram, rom, mem_byte_count(mem_cell_count));

    // The actual inner interpreter.
    // Assumes the starting point is at 1st definition
    do {
        // Push the next addr
        rpush(ram);
        ram[rstk_ptr] = inst_ptr + 1;

        // Jump to header pointer
        inst_ptr = ram[inst_ptr];

        // Jump to entry header
        inst_ptr = ram[inst_ptr];

        if (entry_is_prim(ram)) {
            Cell primidx = entry_primidx(ram);
            primitives[primidx](ram);

            // Jump to next addr
            inst_ptr = ram[rstk_ptr];
            rpop(ram);
        }
        else {
            // Jump to 1st def
            inst_ptr = entry_1stdefn(ram);
            inst_ptr = ram[inst_ptr];
        }
    } while (!exit_requested);
    

    return 0;
}
*/
#endif
