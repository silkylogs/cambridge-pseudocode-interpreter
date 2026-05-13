/*
# CPI C: VM

## Motivation
I wish to bring a language that I've written on paper, by hand, to life. Even if I dislike the semantics of said language very much.

## 11 May 2026
I ought to list as many requirements as I could think of so I don't forget about them later. This are rough notes.
- The development environment should be as portable and as pain-free as possible
  - Stick to a language that is available on at least Windows, Linux and Mac: C89
  - Use the most minimal possible build system (A shell script wrapping something like `CC main.c -o cpi $(FLAGS)`)
  - Be able to painlessly hot-reload on any platform. The easiest way to do that is to write a VM, then write tooling and software for it.
- IO will be from
  - disk (input, output)
  - IO devices (stdout and stdin)
  - memory (read, write)
- Operations in the language are observed to take the following shape
  - Binary operators
  - Unary operators
  - Functions with arbitary number of arguments
  - Procedures with arbitary number of arguments
  - Or, simplifying, functions with arbitary number of arguments and [0, 1] number of return values
- Builtin operations
  - Arithmetic (+ - * /) (Recommend three way operations)
  - IO call
  - Proc calls

## 12 May 2026
The implementation of the VM will be simple. I plan this to be a registerless architecture.

I'd go on to describe it, but at present, a most peculiar bug has caught my attention.
I caught this by pasting yesterday's code into Compiler Explorer. The code ran just fine on my Mac with Clang (and also now on my PC), but it segfaults on CE. Running the address sanitizer (`-fsanitize=address`) revealed my fears. Running it again on my Windows PC has produced the similar result.
```
=================================================================
==6948==ERROR: AddressSanitizer: heap-buffer-overflow on address 0x12d98f4a0030 at pc 0x7ff64ecf194b bp 0x00e0568ff780 sp 0x00e0568ff7c8
WRITE of size 8 at 0x12d98f4a0030 thread T0
    #0 0x7ff64ecf194a in cpirom_append censored/cambridge-pseudocode-interpreter/cpi_c_vm/main.c:137:20
    #1 0x7ff64ecf1f6a in run_tests censored/cambridge-pseudocode-interpreter/cpi_c_vm/main.c:178:23
    #2 0x7ff64ecf222f in main censored/cambridge-pseudocode-interpreter/cpi_c_vm/main.c:205:11
    #3 0x7ff64ecf10fd in __tmainCRTStartup D:/W/B/src/mingw-w64/mingw-w64-crt/crt/crtexe.c:236:11
    #4 0x7ff64ecf13b5 in .l_start D:/W/B/src/mingw-w64/mingw-w64-crt/crt/crtexe.c:122:9
    #5 0x7ffb612ce8d6  (C:\WINDOWS\System32\KERNEL32.DLL+0x18002e8d6)
    #6 0x7ffb6277bf2b  (C:\WINDOWS\SYSTEM32\ntdll.dll+0x1800bbf2b)

0x12d98f4a0031 is located 0 bytes after 1-byte region [0x12d98f4a0030,0x12d98f4a0031)
allocated by thread T0 here:
    #0 0x7ffb1613ba06 in realloc (C:\msys64\clang64\bin\libclang_rt.asan_dynamic-x86_64.dll+0x18004ba06)
    #1 0x7ff64ecf1831 in cpirom_append censored/cambridge-pseudocode-interpreter/cpi_c_vm/main.c:133:8
    #2 0x7ff64ecf1f6a in run_tests censored/cambridge-pseudocode-interpreter/cpi_c_vm/main.c:178:23
    #3 0x7ff64ecf222f in main censored/cambridge-pseudocode-interpreter/cpi_c_vm/main.c:205:11
    #4 0x7ff64ecf10fd in __tmainCRTStartup D:/W/B/src/mingw-w64/mingw-w64-crt/crt/crtexe.c:236:11
    #5 0x7ff64ecf13b5 in .l_start D:/W/B/src/mingw-w64/mingw-w64-crt/crt/crtexe.c:122:9
    #6 0x7ffb612ce8d6  (C:\WINDOWS\System32\KERNEL32.DLL+0x18002e8d6)
    #7 0x7ffb6277bf2b  (C:\WINDOWS\SYSTEM32\ntdll.dll+0x1800bbf2b)

SUMMARY: AddressSanitizer: heap-buffer-overflow censored/cambridge-pseudocode-interpreter/cpi_c_vm/main.c:137:20 in cpirom_append
Shadow bytes around the buggy address:
  0x12d98f49fd80: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x12d98f49fe00: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x12d98f49fe80: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x12d98f49ff00: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x12d98f49ff80: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
=>0x12d98f4a0000: fa fa 00 00 fa fa[01]fa fa fa fa fa fa fa fa fa
  0x12d98f4a0080: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
  0x12d98f4a0100: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
  0x12d98f4a0180: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
  0x12d98f4a0200: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
  0x12d98f4a0280: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
Shadow byte legend (one shadow byte represents 8 application bytes):
  Addressable:           00
  Partially addressable: 01 02 03 04 05 06 07
  Heap left redzone:       fa
  Freed heap region:       fd
  Stack left redzone:      f1
  Stack mid redzone:       f2
  Stack right redzone:     f3
  Stack after return:      f5
  Stack use after scope:   f8
  Global redzone:          f9
  Global init order:       f6
  Poisoned by user:        f7
  Container overflow:      fc
  Array cookie:            ac
  Intra object redzone:    bb
  ASan internal:           fe
  Left alloca redzone:     ca
  Right alloca redzone:    cb
==6948==ABORTING
```

It is just as I feared. My "fix" was really a band-aid. Let me unpack what really happened here.

```c
void cpirom_append(CpiRom *rom, CpiWord what) {
	CpiWord *mem;

	rom->cnt += 1;

	// In the memory reigon allocated here, fair enough
	mem = realloc(rom->mem, rom->cnt); 
	// Wait... did I really allocate `rom->cnt` bytes when I meant to allocate `rom->cnt * CpiWordsz` bytes?

	if (mem) rom->mem = mem;
	else printf("Alloc failure: %zu bytes\n", rom->cnt * CpiWordsz);

	mem[rom->cnt - 1] = what; // WRITE of size 8 at 0x12d98f4a0030 thread T0
}
```
Applying the patch, no errors are found. With that said, the time has come to clean up the code.

```
typedef struct CpiVm CpiVm;
struct CpiVm {
	CpiWord *mem;
	size_t cnt;
};
```
I wish to rename this to `CpiMem`. Here are the pros I can see doing this:
- Name follows usage (It literally is used as a container for memory)
- Frequently used variables (e.g. the instruction pointer) get their own memory address instead of me calculating it by hand, and possibly getting it wrong. Trying to reduce the surface area for attacks, that is.

And the cons:
- I lose the ability to treat the entirety of my program data as a simple array of `uint64_t`s.

Perhaps I can come up with a compromise:
```
// The intended usage of this being memcpy-ed into an existing block of memory, which I can interpret how I wish.
struct CpiVm {
	CpiWord reg_ip;
	CpiWord reg_reserved[0xE];
	
	CpiWord mem[];
};

void *mem = malloc(...);
CpiVm *v = mem;

v->reg_ip = ...;
v->mem[...] = ...;
```

TODO: Allocation.
To make allocation slightly more verbose (and hence easier on me), I should start using an allocator API. I'll copy whatever impresssion I had mucking around the first version of Zig.
```
struct Allocator {
	size_t len;
	size_t cap;
	void *data;
};

Allocator alloc_new(size_t bytes);
void alloc_free(Allocator *);
void alloc_realloc(Allocator *);
```

I don't know how I reached this spot but I will play whack a mole with ASAN another day.
*/


#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

typedef uint64_t CpiWord;
const size_t CpiWordsz = sizeof (CpiWord);
typedef unsigned char CpiByte;

typedef struct CpiVm CpiVm;
struct CpiVm {
	CpiWord reg_ip;
	CpiWord reg_hostmem_len;
	CpiWord reg_hostmem_cap;
	CpiWord reg_reserved[0xC];
	
	CpiByte mem[1024];
};

// -- Instructions --
// Note: for parameters, check implementation (i.e. cpivm_instr_impl_*)

typedef struct CpiVmInstrParams_print_zstr CpiVmInstrParams_print_zstr;
struct CpiVmInstrParams_print_zstr {
	CpiWord zstr;
};

void cpivm_instr_impl_print_zstr(CpiVm *vm, CpiVmInstrParams_print_zstr params) {
	char *zstr = (char*)vm->mem + params.zstr;
	printf("%s", zstr);
}

typedef CpiByte CpiVmInstrIdx;
const CpiVmInstrIdx 
	CPIVM_INSTR_IDX_ZERO = 0,
	CPIVM_INSTR_IDX_PRINT_ZSTR = 1,
	CPIVM_INSTR_IDX_ENUM_COUNT = 2;

// ISO C compat wastes space
typedef struct CpiVmInstr CpiVmInstr;
struct CpiVmInstr {
	CpiVmInstrIdx instr_idx;
	CpiByte padding[sizeof (CpiWord) - 1];
	union Parameters {
		CpiVmInstrParams_print_zstr print_zstr;
	} params;
};

void cpivm_instr_impl_zero(void) {
	printf("\nInvalid instruction index: %2.2x Halting.\n", 0);
	exit(0);
}

// -- Instruction decoder --

CpiVmInstr cpivm_decode(CpiVm *v) {
	CpiVmInstr ins;
	ins = *(CpiVmInstr*)(v->mem + v->reg_ip);

	// Set the active member of the union because ISO C said so
	switch (ins.instr_idx) {
	case CPIVM_INSTR_IDX_ZERO: break; 
	case CPIVM_INSTR_IDX_PRINT_ZSTR: ins.params.print_zstr = ins.params.print_zstr; break;
	case CPIVM_INSTR_IDX_ENUM_COUNT: break;
	default: break;
	}

	return ins;
}


void cpivm_step(CpiVm *v) {
	CpiWord curr_ins_size = 0;
	CpiVmInstr ins = cpivm_decode(v);

	switch(ins.instr_idx) {
	case CPIVM_INSTR_IDX_ZERO: {
		cpivm_instr_impl_zero();
	} break; 
	case CPIVM_INSTR_IDX_PRINT_ZSTR: {
		cpivm_instr_impl_print_zstr(v, ins.params.print_zstr);
	} break;
	default: {
		printf("\nInvalid instruction index: %2.2x Halting.\n", ins.instr_idx);
		exit(0);
	} break;
	}
}


// -- Testing --

bool run_tests(void) {
	CpiVm *v = calloc(1, sizeof (CpiVm));

	CpiByte rom[] = { 
		'\0', 'H', 'e', 'l', 'l', 'o', ' ', ' ', 
		'w', 'o', 'r', 'l', 'd', '\r', '\n', '\0',
		CPIVM_INSTR_IDX_PRINT_ZSTR, 0, 0, 0, 0, 0, 0, 0, 1,
	};
	memcpy(v->mem, rom, sizeof rom);

	v->reg_ip = 16;
	cpivm_step(v);

	return true;
}

// -- Main --

int main(int argc, char **argv) {
	if (CPI_RUN_TESTS) {
		return !run_tests();
	} else {
		return 0;
	}
}
