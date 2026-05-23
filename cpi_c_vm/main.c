int printf(const char *, ...);
void *memcpy(void *dst, const void *src, unsigned long sz);
void exit(int);
#define CHAR_BIT 8

// Defined for sizeof int == 4 * sizeof char. Undefined for anything else.
typedef unsigned int word;
typedef unsigned char byte;

int write_byte(byte *adr, byte what) {
	*adr = what;
	return 1;
}

int write_word(byte *adr, word what) {
	adr[0] = (what >> (0 * 4)) & 0xFF;
	adr[1] = (what >> (1 * 4)) & 0xFF;
	adr[2] = (what >> (2 * 4)) & 0xFF;
	adr[3] = (what >> (3 * 4)) & 0xFF;
	return 4;
}

int read_byte(byte *adr, byte *out) {
	*out = *adr;
	return 1;
}

int read_word(byte *adr, word *out) {
	*out = 0;
	*out |= ((word)adr[0]) << (word)(0 * 4);
	*out |= ((word)adr[1]) << (word)(1 * 4);
	*out |= ((word)adr[2]) << (word)(2 * 4);
	*out |= ((word)adr[3]) << (word)(3 * 4);
	return 4;
}

typedef struct Vm Vm;
struct Vm {
	byte vm_mem[1 << 31]; // Whatever a uint32 can address
	word vm_gpr[1 << 7]; // Whatever a byte can address

	word vm_rip;
	word vm_rflags;
};


typedef struct Parameters Parameters;
struct Parameters {
	byte instr;
	byte src_rmab, dst_rmab; // Tag
	byte src_r, dst_r; // Register
	word src_m, dst_m; // Memory address
	word src_aptr, src_alen, dst_aptr, dst_alen; // (Pointer, len) array
	byte src_b, dst_b; // Byte
	byte op; // Operator
};

const int INVALID = -0xFFFFFFFF;

// Instructions:
// --------------
// Key:
// (register/memory address/(ptr,sz) array/byte)
// _ = blank or invalid
// --------------
//
// Arithmetic (monadic): 2 // It does not make sense to assign an array to a byte
// - r/m/a/_ = op r/m/_/b 
// - a = op a
//
// Arithmetic (dyadic): 3
// - r/m/a/_ = r/m/_/b op r/m/_/b
// - a = a op a
//
// Proc call: 4
// - r/m/a/_
//
// Proc return: 5
// 
// External proc call: 6
// - r/m/a/_
int instr_write(byte *adr, Parameters p) {
	word sz = 0;

	// Base instruction
	adr[sz] = instr;
	sz += 1;

	if (instr == 0) {
		// Zero trap
		return sz;
	} else if (instr == 1) {
		// Assignment: r/m/a/b = r/m/a/b
		
		// Disallow b = a
		if (p.src_rmab == 3 && p.dst_rmab == 2) return INVALID;
		adr[sz] = p.src_rmab;
		sz += 1;

		if (p.src_rmab == 0) {
			sz += write_byte(adr, p.src_r);
		} else if (p.src_rmab == 1) {
			sz += write_word(adr, p.src_m);
		} else if (src_rmab == 2) {
			sz += write_word(adr, p.src_aptr);
			sz += write_word(adr, p.src_alen);
		} else {
			return INVALID;
		}

		if (p.dst_rmab == 0) {
			sz += write_byte(adr, p.dst_r);
		} else if (p.dst_rmab == 1) {
			sz += write_word(adr, p.dst_m);
		} else if (p.dst_rmab == 2) {
			sz += write_word(adr, p.dst_aptr);
			sz += write_word(adr, p.dst_alen);
		} else if (p.dst_rmab == 3) {
			sz += write_byte(adr, p.dst_b);
		} else {
			return INVALID;
		}

		return sz;
	} else {
		return INVALID;
	}

}

/*

// Assumes little endian. 
word vm_ass_reg_to_word(byte *reg) {
	word out = 0, i = 0;
	for (i = 0; i < sizeof (word); ++i) {
		out += (word)reg[i] << (i * CHAR_BIT);
	}
	return out;
}

// Assumes little endian. 
void vm_ass_word_to_reg(word ul, byte *reg) {
	word i = 0;
	byte mask = ~0;
	for (i = 0; i < sizeof(word); ++i) {
		reg[i] = (ul >> (i * CHAR_BIT)) & (word)mask;
	}
}

void vm_zero_regs(void) {
	vm_ass_word_to_reg(0, vm_r0);
	vm_ass_word_to_reg(0, vm_r1);
	vm_ass_word_to_reg(0, vm_r2);
	vm_ass_word_to_reg(0, vm_r3);
	vm_ass_word_to_reg(0, vm_r4);
}

void vm_dbg_print(void) {
	int isz = sizeof(word) * 2;
	word i;
	char status[sizeof (word) * CHAR_BIT] = "THIS_IS_A_PLACEHOLDER_FLAG_STATUS_TODO_ACTUALLY_PLAN_SOMETHING!!";

	printf("\n{");
		// TODO: Figure out why zero padding is incomplete on appleclang 
		printf("\n\trx/r0 = %*.*llx,", isz, isz, vm_ass_reg_to_word(vm_r0));
		printf("\n\try/r1 = %*.*llx,", isz, isz, vm_ass_reg_to_word(vm_r1));
		printf("\n\tra/r2 = %*.*llx,", isz, isz, vm_ass_reg_to_word(vm_r2));
		printf("\n\tri/r3 = %*.*llx,", isz, isz, vm_ass_reg_to_word(vm_r3));
		printf("\n\trf/r4 = ");

		for (i=0; i<sizeof(word)*CHAR_BIT; ++i) {
			word x = (vm_ass_reg_to_word(vm_r4) >> i) & 1;
			printf("%c", x ? status[i] : '.');
		}
	printf("\n}\n");
}

void vm_increment(byte *reg) {
	word tmp;

	tmp = vm_ass_reg_to_word(reg);
	tmp += 1;
	vm_ass_word_to_reg(tmp, reg);
}

byte vm_load_byte(byte *reg) {
	word ptr;
	ptr = vm_ass_reg_to_word(reg);
	return vm_mem[ptr];
}

byte *vm_get_reg(byte idx) {
	switch (idx) {
		case 0: return vm_r0;
		case 1: return vm_r1;
		case 2: return vm_r2;
		case 3: return vm_r3;
		case 4: return vm_r4;
		default:
			printf("\nError: invalid register index %d\n", idx);
			vm_dbg_print();
			exit(2);
	}
}

// Print ip, error message, exit. 
const byte ins_zero[] = { 0, }; 
void impl_zero(void) {
	printf("\nError: Zero trap.\n");
	vm_dbg_print();
	//exit(1);
}

// Prints a null terminated string pointed to by rx. 
const byte ins_print_zstr[] = { 1, };
void impl_print_zstr(void) {
	word idx = 0;

	idx = vm_ass_reg_to_word(vm_r0);
	printf("%s", &vm_mem[idx]);
	vm_increment(vm_r3);
}

// Prints ry characters starting from rx. 
const byte ins_print_lstr[] = { 2, }; 
void impl_print_lstr(void) {
	word start = 0, len = 0, i = 0;

	start = vm_ass_reg_to_word(vm_r0);
	len = vm_ass_reg_to_word(vm_r1);

	for (i = 0; i < len; ++i) {
		printf("%c", vm_mem[start + i]);
	}
	
	vm_increment(vm_r3);
}

// Assign literal to register. Clears the high bytes. 
word ins_write_assign_lit_byte(byte *out, byte reg_idx, byte literal) {
	byte *start = out;
	*out = 3; out++;
	*out = reg_idx; out++;
	*out = literal; out++;
	return out - start;
}

void impl_assign_lit_byte(void) {
	byte *dst_reg=0, tmp=0;

	vm_increment(vm_r3);
	tmp = vm_load_byte(vm_r3);
	dst_reg = vm_get_reg(tmp);

	vm_increment(vm_r3);
	tmp = vm_load_byte(vm_r3);
	vm_ass_word_to_reg(0, dst_reg);
	dst_reg[0] = tmp;

	vm_increment(vm_r3);
}

int main(void) {
#ifdef CPI_RUN_TESTS
	int test_idx = 1;
	for (test_idx = 1; test_idx <= 5; ++test_idx) {
		printf("\n===[test_idx %d]===\n", test_idx);

		if (test_idx == 1) {
			printf("Hello world\n");
		} else if (test_idx == 2) {
			vm_zero_regs();

			vm_mem = (byte *)ins_zero;

			printf("Before: "); vm_dbg_print();
			impl_zero();
			printf("After: "); vm_dbg_print();
		} else if (test_idx == 3) {
			const byte test_ins_print_zstr[] = { 1, 'h', 'e', 'l', 'l', 'o', ' ', 'w', 'o', 'r', 'l', 'd', '\r', '\n', 0, };
			vm_zero_regs();

			vm_mem = (byte *)test_ins_print_zstr;
			vm_ass_word_to_reg(1, vm_r0);

			printf("Before: "); vm_dbg_print();
			impl_print_zstr();
			printf("After: "); vm_dbg_print();
		} else if (test_idx == 4) {
			const byte test_ins_print_lstr[] = { 2, 'l', 'o', 'n', 'g', 0, 's', 't', 'r', 'i', 'n', 'g', '\r', '\n', };
			word lit_ptr = 1, lit_len = 13;
			
			vm_zero_regs();
			
			vm_mem = (byte *)test_ins_print_lstr;
			vm_ass_word_to_reg(lit_ptr, vm_r0);
			vm_ass_word_to_reg(lit_len, vm_r1);

			printf("Before: "); vm_dbg_print();
			impl_print_lstr();
			printf("After: "); vm_dbg_print();
		} else if (test_idx == 5) {
			byte test_ins_assign_lit_byte[3*5] = { 0 };
			word a_fib_coffee_dude = 0xAf1bC0FFEEd0000d;
			word i = 0;
			
			vm_zero_regs();

			i += ins_write_assign_lit_byte(test_ins_assign_lit_byte + i, 0, 0x67);
			i += ins_write_assign_lit_byte(test_ins_assign_lit_byte + i, 1, 0x22);
			i += ins_write_assign_lit_byte(test_ins_assign_lit_byte + i, 2, 0x33);
			i += ins_write_assign_lit_byte(test_ins_assign_lit_byte + i, 4, 0xFF);
			i += ins_write_assign_lit_byte(test_ins_assign_lit_byte + i, 3, 0x55);

			vm_mem = (byte *)test_ins_assign_lit_byte;

			vm_ass_word_to_reg(0x12345678, vm_r0);
			vm_ass_word_to_reg(0x9ABCDEF0, vm_r1);
			vm_ass_word_to_reg(a_fib_coffee_dude, vm_r2);
			vm_ass_word_to_reg(0         , vm_r3);
			vm_ass_word_to_reg(0xCAFEBABE, vm_r4);
			printf("Before: "); vm_dbg_print();

			impl_assign_lit_byte();
			printf("Pass 0: "); vm_dbg_print();
			impl_assign_lit_byte();
			printf("Pass 1: "); vm_dbg_print();
			impl_assign_lit_byte();
			printf("Pass 2: "); vm_dbg_print();
			impl_assign_lit_byte();
			printf("Pass 3: "); vm_dbg_print();
			impl_assign_lit_byte();
			printf("Pass 4: "); vm_dbg_print();
		}
	}
#else
	printf("Actually run the program here.\n");
#endif
	return 0;
}

*/
