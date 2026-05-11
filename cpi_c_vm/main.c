#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

typedef struct CpiVm CpiVm;
struct CpiVm {
	void *mem;
	size_t sz, cap;
};

typedef uint64_t CpiNum;
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
			CpiNum start, end;
			CpiVmInstrIoMode mode;
			{
				// mode = cpivm_instr_io_mode(v, ip);
				mode = v.mem[ip + CPIVM_INSTR_IO_MODE];
			}
			{
				// start = cpivm_instr_io_start(v, ip);
				start = v.mem[ip + CPIVM_INSTR_IO_START];
			}
			{
				// size = cpivm_instr_io_size(v, ip);
				size = v.mem[ip + CPIVM_INSTR_IO_SIZE];
			}
			switch (mode) {
				case CPIVM_INSTR_IO_MODE_IODEVICE_STDOUT: {
					printf("Hello from {%p, %zu, %zu}", v.mem, v.sz, v.cap);
					printf("Instruction: {%llu, %llu, %llu}", mode, start, size);
					printf("%*s", v.mem + start, size);
				} break;
				default: {
					printf("Illegal IO mode");
				} break;
			}
		} break;
		default: {
			printf("Illegal instruction");
		} break;
	}
}

typedef struct CpiRom CpiRom;
struct CpiRom {
	uint8_t *mem;
	size_t sz;
};

bool run_tests(void) {
	CpiVm v = {};
	CpiRom rom = {};
	size_t dataloc;

	// Register page
	cpirom_append(&rom, CpiNumsz * 16, 0);

	// Data
	cpirom_append(&rom, sizeof "Hello, world\n", "Hello, world\n");
	cpirom_align_inflate(&rom);

	// Instructions
	dataloc = rom.sz;
	cpirom_append(&rom, CpiNumsz, CPIVM_INSTR_IO);
	cpirom_append(&rom, CpiNumsz, CPIVM_INSTR_IO_MODE_IODEVICE_STDOUT);
	cpirom_append(&rom, CpiNumsz, dataloc);
	cpirom_append(&rom, CpiNumsz, sizeof "Hello, world\n");

	// Run!
	cpivm_allocload(v, rom, romsz);
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
