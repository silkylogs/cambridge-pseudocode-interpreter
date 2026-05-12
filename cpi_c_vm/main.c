/*
# CPI C: VM

## Motivation
I wish to bring a language that I've written on paper, by hand, to life. Even if I dislike the semantics of said language very much.

## 11 May, 2026
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
*/


#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

typedef uint64_t CpiNum;

typedef struct CpiVm CpiVm;
struct CpiVm {
	CpiNum *mem;
	size_t cnt;
};

const size_t CpiNumsz = sizeof (CpiNum);

typedef enum CpiVmReg CpiVmReg;
enum CpiVmReg {
	CPIVM_REG_ZERO,
	CPIVM_REG_IP,
	CPIVM_REG_ENUM_COUNT,
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

void cpivm_step(CpiVm v) {
	CpiNum ip;
	CpiVmInstr t;
	{
		// ip = cpivm_ip(v);
		ip = v.mem[CPIVM_REG_IP];
	}
	{
		// t = cpivm_instr_type(v, ip);
		t = v.mem[ip];
	}
	switch (t) {
		case CPIVM_INSTR_IO: {
			CpiNum start, size;
			CpiVmInstrIoMode mode;

			ip += 1;

			// mode = cpivm_instr_io_mode(v, ip);
			mode = v.mem[ip + CPIVM_INSTR_IO_MODE];
			// start = cpivm_instr_io_start(v, ip);
			start = v.mem[ip + CPIVM_INSTR_IO_START];
			// size = cpivm_instr_io_size(v, ip);
			size = v.mem[ip + CPIVM_INSTR_IO_SIZE];
			switch (mode) {
				case CPIVM_INSTR_IO_MODE_IODEVICE_STDOUT: {
					int sz = size;
					char *str = (char*)(&v.mem[start]);
					printf("CpiVm {v.mem=%p, v.cnt=%zu};\n", v.mem, v.cnt);
					printf("Instruction: {mode=%x, start=%llu, size=%llu}\n", mode, start, size);
					printf("printf((insert string fmt here), sz=%d, sz=%d, str=%p);\n", sz, sz, str);
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

typedef struct CpiRom CpiRom;
struct CpiRom {
	CpiNum *mem;
	size_t cnt;
};

void cpirom_append(CpiRom *rom, CpiNum what) {
	CpiNum *mem;

	rom->cnt += 1;

	mem = realloc(rom->mem, rom->cnt);
	if (mem) rom->mem = mem;
	else printf("Alloc failure: %zu bytes\n", rom->cnt * CpiNumsz);

	mem[rom->cnt - 1] = what;
}

void cpirom_append_zstr_aligned(CpiRom *rom, char *zstr) {
	size_t dest = rom->cnt;
	size_t sz = strlen(zstr) + 1;
	size_t sz_in_cnt_padded = (sz / CpiNumsz) + 1;
	size_t i = 0;

	for (; i < sz_in_cnt_padded; ++i) {
		cpirom_append(rom, 0);
	}

	memcpy(&rom->mem[dest], zstr, sz);
}

void cpirom_print(CpiRom rom) {
	int i;
	printf("[");
	for (i=0; i < rom.cnt * CpiNumsz; ++i) {
		if (i % CpiNumsz == 0) printf("\n%4.4x: ", i);
		char c = *(((char*)rom.mem) + i);
		isalnum(c) ? printf(" %c, ", c) : printf("%2.2X, ", c);
	}
	printf("\n]\n");
}

void cpivm_allocload(CpiVm *vm, CpiRom r) {
	vm->cnt = r.cnt;
	vm->mem = calloc(vm->cnt, CpiNumsz);
	memcpy(vm->mem, r.mem, vm->cnt * CpiNumsz);
}

bool run_tests(void) {
	CpiVm v = {};
	CpiRom rom = {};
	size_t dataloc = 0;
	size_t instrloc = 0;
	size_t i = 0;

	// Register page
	for (i=0; i<16; ++i) cpirom_append(&rom, 0);
	cpirom_print(rom);

	// Data
	dataloc = rom.cnt;
	cpirom_append_zstr_aligned(&rom, "Hello, world\n");
	cpirom_print(rom);
	instrloc = rom.cnt;

	// Instructions
	cpirom_append(&rom, CPIVM_INSTR_IO);
	cpirom_append(&rom, CPIVM_INSTR_IO_MODE_IODEVICE_STDOUT);
	cpirom_append(&rom, dataloc);
	cpirom_append(&rom, sizeof "Hello, world\n");

	cpirom_print(rom);

	// Run!
	cpivm_allocload(&v, rom);
	v.mem[CPIVM_REG_IP] = instrloc;
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
