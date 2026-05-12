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
void cpirom_append(CpiRom *rom, CpiNum what) {
	CpiNum *mem;

	rom->cnt += 1;

	// In the memory reigon allocated here, fair enough
	mem = realloc(rom->mem, rom->cnt); 
	// Wait... did I really allocate `rom->cnt` bytes when I meant to allocate `rom->cnt * CpiNumsz` bytes?

	if (mem) rom->mem = mem;
	else printf("Alloc failure: %zu bytes\n", rom->cnt * CpiNumsz);

	mem[rom->cnt - 1] = what; // WRITE of size 8 at 0x12d98f4a0030 thread T0
}
```
Applying the patch, no errors are found. With that said, the time has come to clean up the code.

```
typedef struct CpiVm CpiVm;
struct CpiVm {
	CpiNum *mem;
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
	CpiNum reg_ip;
	CpiNum reg_reserved[0xE];
	
	CpiNum mem[];
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

typedef uint64_t CpiNum;
const size_t CpiNumsz = sizeof (CpiNum);
typedef unsigned char CpiByte;

typedef struct CpiVm CpiVm;
struct CpiVm {
	CpiNum reg_ip;
	CpiNum reg_reserved[0xE];
	
	CpiByte mem[];
};

typedef enum CpiVmInstr CpiVmInstr;
enum CpiVmInstr {
	CPIVM_INSTR_ZERO,
	CPIVM_INSTR_IO,
	CPIVM_INSTR_ENUM_COUNT,
};

typedef enum CpiVmInstrIo CpiVmInstrIo;
enum CpiVmInstrIo {
	CPIVM_INSTR_IO_MODE,
	CPIVM_INSTR_IO_START,
	CPIVM_INSTR_IO_SIZE,
};

typedef enum CpiVmInstrIoMode CpiVmInstrIoMode;
enum CpiVmInstrIoMode {
	CPIVM_INSTR_IO_MODE_IODEVICE_STDOUT,
	CPIVM_INSTR_IO_MODE_IODEVICE_STDIN,
	CPIVM_INSTR_IO_MODE_DISK_READ,
	CPIVM_INSTR_IO_MODE_DISK_WRITE,
	CPIVM_INSTR_IO_MODE_MEM_READ,
	CPIVM_INSTR_IO_MODE_MEM_WRITE,
};

void cpivm_step(CpiVm *v) {
	printf("Accessing %llx\n", v->reg_ip);
	CpiVmInstr t = v->mem[v->reg_ip];
	switch (t) {
		case CPIVM_INSTR_IO: {
			CpiNum start, size;
			CpiVmInstrIoMode mode;

			v->reg_ip += 1;

			// mode = cpivm_instr_io_mode(v, ip);
			mode = v->mem[v->reg_ip + CPIVM_INSTR_IO_MODE];
			// start = cpivm_instr_io_start(v, ip);
			start = v->mem[v->reg_ip + CPIVM_INSTR_IO_START];
			// size = cpivm_instr_io_size(v, ip);
			size = v->mem[v->reg_ip + CPIVM_INSTR_IO_SIZE];
			switch (mode) {
				case CPIVM_INSTR_IO_MODE_IODEVICE_STDOUT: {
					int sz = size;
					char *str = (char*)(&v->mem[start]); // May need to translate from word adddressing to byte addressing
					//printf("CpiVm {v.mem=%p, v.cnt=%zu};\n", v.mem, v.cnt);
					//printf("Instruction: {mode=%x, start=%llu, size=%llu}\n", mode, start, size);
					//printf("printf((insert string fmt here), sz=%d, sz=%d, str=%p);\n", sz, sz, str);
					printf("%*.*s", sz, sz, str);
				} break;
				default: {
					printf("Illegal IO mode: %x\n", mode);
				} break;
			}
		} break;
		default: {
			printf("Illegal instruction: %x\n", t);
		} break;
	}
}

// cpibb module (Cpi backing buffer)

void cpibb_appendb(CpiByte **bb, size_t *bbsz, CpiByte what) {
	CpiByte *tmp;

	*bbsz += 1;

	tmp = realloc(*bb, *bbsz);
	if (tmp) *bb = tmp;
	else printf("Alloc failure: %zu bytes\n", *bbsz);

	(*bb)[*bbsz - 1] = what;
}

void cpibb_append_zstr_aligned(CpiByte **bb, size_t *bbsz, char *zstr) {
	CpiByte *tmp;
	size_t sz, oldsz;
       
	sz = strlen(zstr) + 1;
	oldsz = *bbsz;
	*bbsz += sz;

	tmp = realloc(*bb, *bbsz);
	if (tmp) *bb = tmp;
	else printf("Alloc failure: %zu bytes\n", *bbsz);

	memcpy(*bb + oldsz, zstr, sz);
}

void cpibb_print(CpiByte *bb, size_t bbsz) {
	char c;
	int i, col_cnt = 8;

	printf("(Size: %zX) [", bbsz);
	for (i=0; i < bbsz; ++i) {
		if (i % col_cnt == 0) printf("\n\t%4.4x: ", i);
		c = bb[i];
		isalnum(c) ? printf("\'%c, ", c) : printf("%2.2X, ", c);
	}
	printf("\n]\n");
}

bool run_tests(void) {
	CpiVm *v = NULL;
	
	CpiByte *bb = NULL;
	size_t bbsz = 0;

	size_t dataloc = 0;
	size_t instrloc = 0;

	// Reserved for registers
	bbsz = 16 * CpiNumsz;
	bb = calloc(bbsz, sizeof (CpiByte));
	cpibb_print(bb, bbsz);

	// Data
	dataloc = bbsz;
	cpibb_append_zstr_aligned(&bb, &bbsz, "Hello, world\n");
	cpibb_print(bb, bbsz);
	instrloc = bbsz;

	// Instructions. TODO: write a function that R/Ws them for you.
	cpibb_appendb(&bb, &bbsz, CPIVM_INSTR_IO);
	cpibb_appendb(&bb, &bbsz, CPIVM_INSTR_IO_MODE_IODEVICE_STDOUT);
	cpibb_appendb(&bb, &bbsz, dataloc);
	printf("dataloc: %zu\n", dataloc);
	cpibb_appendb(&bb, &bbsz, sizeof "Hello, world\n");
	printf("bbsz: %zu\n", bbsz);
	printf("sizeof hello worl: %d\n", bb[bbsz]);
	cpibb_print(bb, bbsz);

	// Run!
	v = (CpiVm*)(bb);
	v->reg_ip = instrloc;
	cpivm_step(v);

	return true;
}

int main(int argc, char **argv) {
	if (CPI_RUN_TESTS) {
		return !run_tests();
	} else {
		return 0;
	}
}
