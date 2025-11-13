#pragma once

/*
Diagrams generated with: https://asciiflow.com/
┌───────────────────────────────────────────────────────────────────────────┐
│To best illustrate how this forth works, one must understand the dictionary│
│The format of a typical dictionary is as follows:                          │
│┌────┐                                                                     │
││NULL│                                                                     │
│└─▲──┘                                                                     │
│┌─┼──┬─────┬─────────┬─────┬─────────────────────┐       ┌────────────────┐│
││LINK│FLAGS│NAME_SIZE│NAME │PTR_TO_START_OF_ENTRY│       │DEFINITION......││
│└─▲──┴─────┴─────────┴─────┴─────────────────────┘       └────────────────┘│
│┌─┼──┬─────┬─────────┬─────┬─────────────────────┐       ┌────────────────┐│
││LINK│FLAGS│NAME_SIZE│NAME │PTR_TO_START_OF_ENTRY│       │DEFINITION......││
│└─▲──┴─────┴─────────┴─────┴─────────────────────┘       └────────────────┘│
│┌─┼──┬─────┬─────────┬─────┬─────────────────────┐       ┌────────────────┐│
││LINK│FLAGS│NAME_SIZE│NAME │PTR_TO_START_OF_ENTRY│       │DEFINITION......││
│└─▲──┴─────┴─────────┴─────┴─────────────────────┘       └────────────────┘│
│┌─┼──┬─────┬─────────┬─────┬─────────────────────┐       ┌────────────────┐│
││LINK│FLAGS│NAME_SIZE│NAME │PTR_TO_START_OF_ENTRY│       │DEFINITION......││
│└────┴─────┴─────────┴─────┴─────────────────────┘       └────────────────┘│
└───────────────────────────────────────────────────────────────────────────┘


┌─────────────────────────────────────────────────────────────────────────────────┐
│ Elaborating the exact format of each entry:                                     │
│ Note: Each box represents a "Cell", which is the "bit-ness" of the forth.       │
│       This forth has 16 bits in a cell, or 2 bytes.                             │
│ ────────────────────────────────────────────────────────────────────────────    │
│                                                                                 │
│┌────────────┐┌─────┐┌─────────┐┌───────┐┌───┐┌───────┐┌─────────┐               │
││LINK POINTER││FLAGS││NAME_SIZE││NAME[0]││...││NAME[N]││ENTRY_PTR│               │
│└───────▲────┘└─────┘└─────────┘└───────┘└───┘└───────┘└────┬────┘               │
│        └───────────────────────────────────────────────────┘                    │
└─────────────────────────────────────────────────────────────────────────────────┘
┌─────────────────────────────────────────────────────────────────────────────────┐
│┌────────┐                                                                       │
││LINK_PTR│ (Link pointer)                                                        │
│└────────┘                                                                       │
│Points to a) The next word                                                       │
│          b) NULL, indicating the end of the list.                               │
│                                                                                 │
│Remark: This makes the word-search algorithm simple:                             │
│       ┌──────────────────────────────────────┐                                  │
│       │while (ptr) {                         │                                  │
│       │        if (name(ptr) == query) break;│                                  │
│       │        else ptr = *ptr;              │                                  │
│       │}                                     │                                  │
│       │return ptr;                           │                                  │
│       └──────────────────────────────────────┘                                  │
└─────────────────────────────────────────────────────────────────────────────────┘
┌─────────────────────────────────────────────────────────────────────────────────┐
│┌─────┐                                                                          │
││FLAGS│                                                                          │
│└─────┘                                                                          │
│Contains data on how to interpret this word.                                     │
│┌─┬─┬───────────────────────────┐                                                │
││F│E│D C B A 9 8 7 6 5 4 3 2 1 0│                                                │
│└┬┴┬┴┬──────────────────────────┘                                                │
│ │ │ └────────────────►Reserved                                                  │
│ │ └──────────────────►Immediate (does nothing, for now)                         │
│ └────────────────────►Primitive (Calls an external function)                    │
└─────────────────────────────────────────────────────────────────────────────────┘
┌─────────────────────────────────────────────────────────────────────────────────┐
│┌─────────┐                                                                      │
││NAME_SIZE│                                                                      │
│└─────────┘                                                                      │
│ Contains the length of the name of the word, (in words),                        │
│ Aligned to the nearest word.                                                    │
│ The name is not zero terminated, though is padded with zeroes to align to the   │
│ nearest word.                                                                   │
│                                                                                 │
│ Remark: This size is needed to get to the ENTRY_PTR field and beyond.           │
│                                                                                 │
│┌───────┐┌───┐┌───────┐                                                          │
││NAME[0]││...││NAME[N]│                                                          │
│└───────┘└───┘└───────┘                                                          │
│Examples of valid NAME_SIZEs and NAMEs:                                          │
│┌────┐   ┌────┐┌────┐┌────┐                                                      │
││0003│   │F O ││R T ││H \0│                                                      │
│└────┘   └────┘└────┘└────┘                                                      │
│┌────┐   ┌────┐┌────┐┌────┐┌────┐┌────┐┌────┐┌────┐┌────┐┌────┐┌────┐┌────┐┌────┐│
││000B│   │P s ││e u ││d o ││h y ││p o ││p a ││r a ││t h ││y r ││o i ││d i ││s m ││
│└────┘   └────┘└────┘└────┘└────┘└────┘└────┘└────┘└────┘└────┘└────┘└────┘└────┘│
└─────────────────────────────────────────────────────────────────────────────────┘
┌─────────────────────────────────────────────────────────────────────────────────┐
│┌─────────┐                                                                      │
││ENTRY_PTR│ (Entry pointer)                                                      │
│└─────────┘                                                                      │
│ Self explanatory. Points to the first cell of the entry. i.e. LINK_PTR          │
│ Remark: Needed for the double dereference of the inner interpreter to work.     │
└─────────────────────────────────────────────────────────────────────────────────┘
┌────────────────────────────────────────────────────────────────────────────────────────────────────┐
│ ┌───────────────────────────────────────┐                                                          │
│ │Forth inner interpreter operating cycle│                                                          │
│ └───────────────────────────────────────┘                                                          │
│                                                                                                    │
│  Consider the following dictionary:                                                                │
│     ┌────┐                                                                                         │
│     │NULL│                                                                                         │
│     └─▲──┘                                                                                         │
│ ┌─────┼─────◄┬┬───┬┬─────────┬┬────┬┬────┬┬─────────┐┌──────────────────────────────────┐          │
│ │LINK POINTER││I=1││NAME_SZ=2││D U ││P \0││ENTRY_PTR││<index to implementation function>│          │
│ └─────▲──────┘└───┘└─────────┘└────┘└────┘└─────────┘└──────────────────────────────────┘          │
│ ┌─────┼─────◄┬┬───┬┬─────────┬┬────┬┬────┬┬─────────┐┌──────────────────────────────────┐          │
│ │LINK POINTER││I=1││NAME_SZ=2││R E ││T \0││ENTRY_PTR││<index to implementation function>│          │
│ └─────▲──────┘└───┘└─────────┘└────┘└────┘└─────────┘└──────────────────────────────────┘          │
│ ┌─────┼─────◄┬┬───┬┬─────────┬┬────┬┬─────────┐┌──────────────────────────────────┐                │
│ │LINK POINTER││I=1││NAME_SZ=1││+ \0││ENTRY_PTR││<index to implementation function>│                │
│ └────────────┘└───┘└─────────┘└────┘└─────────┘└──────────────────────────────────┘                │
│ ┌─────┼─────◄┬┬───┬┬─────────┬┬────┬┬─────────┐┌───┐┌─┐┌───┐                                       │
│ │LINK POINTER││I=1││NAME_SZ=1││2 x ││ENTRY_PTR││DUP││+││RET│                                       │
│ └────────────┘└───┘└─────────┘└────┘└─────────┘└───┘└─┘└───┘                                       │
│ ┌─────┼─────◄┬┬───┬┬─────────┬┬────┬┬─────────┐┌──┐┌──┐┌───┐                                       │
│ │LINK POINTER││I=1││NAME_SZ=1││4 x ││ENTRY_PTR││2x││2x││RET│                                       │
│ └────────────┘└───┘└─────────┘└────┘└─────────┘└──┘└──┘└───┘                                       │
│                                                                                                    │
│   And consider the following variables:                                                            │
│                                                                                                    │
│1. Instruction pointer (abbreviated IP from now on): points to the LINK POINTER of 4x               │
│2. Return stack:       (abbreviated RS from now on): empty                                          │
│3. Data stack:         (abbreviated DS from now on): [ 2 ]                                          │
│                                                                                                    │
│High level description:                                                                             │
│0. Suppose we've typed the following prompt into the interpreter: `2 4x .`                          │
│   i.e. "push 2 onto DS, multiply by four, print the result"                                        │
│   The interpreter has consumed the word "4x", walked down the link pointers, and found the result. │
│   **THE INITIAL STATE IS WHEN THE INSTRUCTION POINTER IS AT THE LINK POINTER OF THE FOUND WORD.**  │
│   ┌────────────────────────────────────────┐                                                       │
│   │IP = link_ptr_of_4x                     │                                                       │
│   │RS = []                                 │                                                       │
│   │DS = [2]                                │                                                       │
│   └────────────────────────────────────────┘                                                       │
│                                                                                                    │
│1. Get the offset address of the first definition of that word using the formula                    │
│   ┌─────────────────────────────────────────────────────┐                                          │
│   │ 1 + 1 + NAME_SZ + 1 ◄───Cell containing ENTRY_PTR   │                                          │
│   │ ▲   ▲         ▲                                     │                                          │
│   │ │   │         └─────────Cells containing the name   │                                          │
│   │ │   └───────────────────Cell containing size of name│                                          │
│   │ └───────────────────────Cell containing flags       │                                          │
│   └─────────────────────────────────────────────────────┘                                          │
│                                                                                                    │
│2. Jump to the first definition of that word                                                        │
│   ┌────────────────────────────────────────┐                                                       │
│   │IP = 2x                                 │                                                       │
│   │RS = []                                 │                                                       │
│   │DS = [2]                                │                                                       │
│   └────────────────────────────────────────┘                                                       │
│                                                                                                    │
│3. Save the address of the next cell to return to, which is (IP + 1) 2                              │
│   ┌────────────────────────────────────────┐                                                       │
│   │IP = 2x                                 │                                                       │
│   │RS = [2x] (Note: we call 2x twice)      │                                                       │
│   │DS = [2]                                │                                                       │
│   └────────────────────────────────────────┘                                                       │
│                                                                                                    │
│4. Jump to wherever that cell points to, which is the entry pointer of 2x                           │
│   ┌────────────────────────────────────────┐                                                       │
│   │IP = entry_ptr_of_2x                    │                                                       │
│   │RS = [2x] (Note: we call 2x twice)      │                                                       │
│   │DS = [2]                                │                                                       │
│   └────────────────────────────────────────┘                                                       │
│                                                                                                    │
│5. Jump to wherever the entry pointer points to, which is...                                        │
│   the link pointer of 2x! We've come full circle!                                                  │
│   ┌────────────────────────────────────────┐                                                       │
│   │IP = link_ptr_of_2x                     │                                                       │
│   │RS = [2x]                               │                                                       │
│   │DS = [2]                                │                                                       │
│   └────────────────────────────────────────┘                                                       │
│                                                                                                    │
│Continuing the cycle:                                                                               │
│                                                                                                    │
│   ┌────────────────────────────────────────┐                                                       │
│0. │IP = link_ptr_of_2x                     │                                                       │
│   │RS = [2x]                               │                                                       │
│   │DS = [2]                                │                                                       │
│   └────────────────────────────────────────┘                                                       │
│   ┌────────────────────────────────────────┐                                                       │
│2. │IP = dup                                │                                                       │
│   │RS = [2x]                               │                                                       │
│   │DS = [2]                                │                                                       │
│   └────────────────────────────────────────┘                                                       │
│   ┌────────────────────────────────────────┐                                                       │
│3. │IP = dup                                │                                                       │
│   │RS = [2x +]                             │                                                       │
│   │DS = [2]                                │                                                       │
│   └────────────────────────────────────────┘                                                       │
│   ┌────────────────────────────────────────┐                                                       │
│4. │IP = entry_ptr_of_dup                   │                                                       │
│   │RS = [2x +]                             │                                                       │
│   │DS = [2]                                │                                                       │
│   └────────────────────────────────────────┘                                                       │
│   ┌────────────────────────────────────────┐                                                       │
│5. │IP = link_ptr_of_dup                    │                                                       │
│   │RS = [2x +]                             │                                                       │
│   │DS = [2]                                │                                                       │
│   └────────────────────────────────────────┘                                                       │
│                                                                                                    │
│                                                                                                    │
│These cycles applied when the word being executed was one defined in forth. Now that we arent in    │
│one, now what? Not to worry, we simply get to the first cell constituting the definition and treat  │
│it as an index to a function we've implemented in the implmementation language of choice.           │
│                                                                                                    │
│At this point, you may be wondering how we'd stop ascending up the dictionary, and how can we return│
│from a function defined in forth (the ones that are not primitives).                                │
│I will answer the first one first. Answering the second question should be intutitive then.         │
│                                                                                                    │
│As soon as we finish executing the primitive DUP, the interpreter does the following:               │
│  1. Pop the return stack, moving the popped value to IP.                                           │
│     ┌────────────────────────────────────────┐                                                     │
│     │IP = + ◄───────┐                        │                                                     │
│     │RS = [2x]──────┘                        │                                                     │
│     │DS = [2 2]                              │                                                     │
│     └────────────────────────────────────────┘                                                     │
│                                                                                                    │
│  2. Dereference IP to go to the entry pointer.                                                     │
│     ┌────────────────────────────────────────┐                                                     │
│     │IP = entry_ptr_of_+                     │                                                     │
│     │RS = [2x]                               │                                                     │
│     │DS = [2 2]                              │                                                     │
│     └────────────────────────────────────────┘                                                     │
│                                                                                                    │
│  3. Dereference IP again to go to the link pointer, which is our starting point.                   │
│     ┌────────────────────────────────────────┐                                                     │
│     │IP = link_ptr_of_+                      │                                                     │
│     │RS = [2x]                               │                                                     │
│     │DS = [2 2]                              │                                                     │
│     └────────────────────────────────────────┘                                                     │
│                                                                                                    │
│To answer the second question, think about what we've just done to "return" from a primitive and    │
│apply just that to our forth function: pop-deref-deref.                                             │
│                                                                                                    │
│                                                                                                    │
│    ┌────────────────────────────────────────┬───────────────┐                                      │
│1.  │IP = RET                                │               │                                      │
│    │RS = [2x (RET+1)]                       │               │                                      │
│    │DS = [2 2]                              │               ▼                                      │
│    └────────────────────────────────────────┘ Side effect of RET, the primitive:                   │
│    ┌────────────────────────────────────────┐ Deletes the top of the stack                         │
│2.  │IP = RET                                │                                                      │
│    │RS = [2x]                               │               │                                      │
│    │DS = [2 2]                              │ ◄─────────────┘                                      │
│    └────────────────────────────────────────┘                                                      │
│    ┌────────────────────────────────────────┐                                                      │
│3.  │IP = 2x ◄─┐                             │                                                      │
│    │RS = []───┘                             │                                                      │
│    │DS = [2 2]                              │                                                      │
│    └────────────────────────────────────────┘                                                      │
│                                                                                                    │
│The reader is highly encouraged to trace the movements with pen and paper.                          │
└────────────────────────────────────────────────────────────────────────────────────────────────────┘
*/



#include <stdio.h>
#include <stdint.h>
#include <utility>
#include <array>
#include <string>
#include <vector>
#include <iostream>

using Cell = uint16_t;
using Byte = uint8_t;

namespace Forth {
    struct State {
        Cell* memory;
        Cell mem_allocated;
        Cell mem_used;

        bool exit_requested;

        std::vector<void(*)(State&)> primitives;

        Cell* ptr_instruction;
        Cell* ptr_return_stack;
        Cell* ptr_data_stack;
        Cell* dict_next_available_cell;
        Cell* dict_latest_defined_entry;
    };

    void inner_interpreter(State& self);

    namespace BuiltinPrimitives {
        
    };

    namespace Util {
        void alloc_set_memory_conents(State& self, char* const byte_string, size_t strlen);
        void fixup_ptrs_after_allocation(State& self);
        void add_new_entry(State& self, char* const byte_string, size_t strlen);
        void append_def_to_latest_entry(State& self, char* const byte_string, size_t strlen);
    };
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
Cell& inst_ptr = ram[0x0000];
Cell& dstk_ptr = ram[0x0001];
Cell& rstk_ptr = ram[0x0002];
bool exit_requested = false;

// Primitives

// Note: push semantic: stack pointer always points to the top of the stack.

void exit(Cell* m) { printf("Exiting...\n"); exit_requested = true; };
void nop(Cell* m) { (void)m; printf("nop "); };
void dpush(Cell* m) { dstk_ptr++; }
void dpop(Cell* m) { dstk_ptr--; }
void rpush(Cell* m) { rstk_ptr++; }
void rpop(Cell* m) { rstk_ptr--; }
void lit(Cell* m) {
    dpush(m);
    m[dstk_ptr] = m[m[rstk_ptr]];
    m[rstk_ptr]++;
}
void printchar(Cell* m) { printf("%c", m[dstk_ptr]); dstk_ptr--; }
void brz(Cell* m) {
    Cell addr = m[dstk_ptr];
    dstk_ptr--;
    Cell cond = m[dstk_ptr];
    dstk_ptr--;
    if (!cond) {
        m[rstk_ptr] = addr;
    }
}
void sto(Cell* m) {
    Cell addr = m[dstk_ptr];
    dstk_ptr--;
    Cell val = m[dstk_ptr];
    dstk_ptr--;
    m[addr] = val;
}
void lod_cell(Cell* m) {
    Cell addr = m[dstk_ptr];
    m[dstk_ptr] = m[addr];
}
void inc(Cell* m) { m[dstk_ptr]++; }

// Helper functions

// Forth word entry: [Link ptr][prim=0,immed,len...][name][header ptr][def]...
// Primitive entry:  [Link ptr][prim=1,immed,len...][name][header ptr][primitive index]
bool entry_is_prim(Cell* m) {
    //return *(start_of_entry + 1) & 0x7000;
    return m[inst_ptr + 1] & 0x8000;
}
bool entry_is_immed(Cell* m) {
    return m[inst_ptr + 1] & 0x4000;
}
Cell entry_strlen(Cell* m) {
    return m[inst_ptr + 1] & 0x3FFF;
}
Cell entry_headerptr(Cell* m) {
    return m[inst_ptr + entry_strlen(m) + 2];
}
Cell entry_1stdefn(Cell* m) {
    return m[inst_ptr + entry_strlen(m) + 3];
}
Cell entry_primidx(Cell* m) {
    return m[inst_ptr + entry_strlen(m) + 3];
}

Cell entry_linkptr(Cell* m) {
    return m[inst_ptr];
}

// Also doubles as instruction set... technically
std::array<void(*)(Cell*), 12> primitives{
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
