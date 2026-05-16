int printf(const char *, ...);
void *memcpy(void *dst, const void *src, unsigned long int sz);
void exit(int);

/* Assuming a byte is (at least) eight bits */
unsigned char 
	*vm_mem, /* Memory. */
	vm_r0[2], /* Register X. Memory. */
	vm_r1[2], /* Register Y. Memory. */
	vm_r2[2], /* Accumulator. Arithmetic. */
	vm_r3[2], /* Instruction pointer. */
	vm_flags; /* Branch conditionals. */

unsigned char vm_container[1024];

void vm_dbg_print(void) {
	int irx=0, iry=0, ira=0, iri=0, irf=0;

	memcpy(&irx, vm_r0, 2);
	memcpy(&iry, vm_r1, 2);
	memcpy(&ira, vm_r2, 2);
	memcpy(&iri, vm_r3, 2);
	memcpy(&irf, &vm_flags, 1);

	printf("\n{rx=%4.4x, ry=%4.4x, ra=%4.4x, ri=%4.4x, flags=", irx, iry, ira, iri);

	printf("%c", ((irf >> 0) & 1) ? '?' : '.');
	printf("%c", ((irf >> 1) & 1) ? '?' : '.');
	printf("%c", ((irf >> 2) & 1) ? '?' : '.');
	printf("%c", ((irf >> 3) & 1) ? '?' : '.');
	printf("%c", ((irf >> 4) & 1) ? '?' : '.');
	printf("%c", ((irf >> 5) & 1) ? '?' : '.');
	printf("%c", ((irf >> 6) & 1) ? '?' : '.');
	printf("%c", ((irf >> 7) & 1) ? '?' : '.');

	printf("}\n");
}

void vm_increment(unsigned char *reg) {
	unsigned x = 0;
	memcpy(&x, reg, 2);
	x++;
	memcpy(reg, &x, 2);
}

unsigned long long int vm_deref1(unsigned char *reg) {
	unsigned long long int ptr=0, rv=0;
	memcpy(&ptr, reg, 2);

	rv = vm_mem[ptr];
	return rv;
}

unsigned char *vm_get_reg(unsigned char idx) {
	switch (idx) {
		case 0: return vm_r0;
		case 1: return vm_r1;
		case 2: return vm_r2;
		case 3: return vm_r3;
		default:
			printf("\nError: invalid register index %d\n", idx);
			vm_dbg_print();
			exit(2);
	}
}

/* Print ip, error message, exit. */
const unsigned char ins_zero[] = { 0, }; 
void impl_zero(void) {
	printf("\nError: Zero trap.\n");
	vm_dbg_print();
	exit(1);
}

/* Null terminated string pointed to by rx. */
const unsigned char ins_print_zstr[] = { 1, };
void impl_print_zstr(void) {
	unsigned long long int idx = 0;

	memcpy(&idx, vm_r0, 2);
	printf("%s", &vm_mem[idx]);
	vm_increment(vm_r3);
}

enum Placeholders {
	P_LO, /* Parameter low byte. */
	P_HI, /* Parameter high byte. */
};

/* Prints ry characters starting from rx. */
const unsigned char ins_print_lstr[] = { 2, }; 
void impl_print_lstr(void) {
	unsigned long long int start = 0;
	int len = 0, i = 0;

	memcpy(&start, vm_r0, 2);
	memcpy(&len, vm_r1, 2);
	for (i = 0; i < len; ++i) {
		printf("%c", vm_mem[start + i]);
	}
	vm_increment(vm_r3);
}

/* Assign literal to register. Clears the high byte.  */
const unsigned char ins_assign_lit1[] = { 3, P_LO, P_LO, };
void impl_assign_lit1(void) {
	unsigned char *dst_reg=0, tmp=0;

	vm_increment(vm_r3);
	tmp = vm_deref1(vm_r3);
	dst_reg = vm_get_reg(tmp);

	vm_increment(vm_r3);
	dst_reg[0] = vm_deref1(vm_r3);
	dst_reg[1] = 0;

	vm_increment(vm_r3);
}

int main() {
	if (CPI_RUN_TESTS == 1) {
		printf("Hello world\n");
	} else if (CPI_RUN_TESTS == 2) {
		vm_mem = (unsigned char *)ins_zero;
		impl_zero();
	} else if (CPI_RUN_TESTS == 3) {
		const unsigned char test_ins_print_zstr[] = { 1, 'h', 'e', 'l', 'l', 'o', ' ', 'w', 'o', 'r', 'l', 'd', '\r', '\n', 0, };
		int lit_ptr = 1;

		vm_mem = (unsigned char *)test_ins_print_zstr;
		memcpy(vm_r0, &lit_ptr, 2);

		printf("Before: "); vm_dbg_print();
		impl_print_zstr();
		printf("After: "); vm_dbg_print();
	} else if (CPI_RUN_TESTS == 4) {
		const unsigned char test_ins_print_lstr[] = { 2, 'l', 'o', 'n', 'g', 0, 's', 't', 'r', 'i', 'n', 'g', '\r', '\n', };
		int lit_ptr = 1, lit_len = 13;
		
		vm_mem = (unsigned char *)test_ins_print_lstr;
		memcpy(vm_r0, &lit_ptr, 2);
		memcpy(vm_r1, &lit_len, 2);

		printf("Before: "); vm_dbg_print();
		impl_print_lstr();
		printf("After: "); vm_dbg_print();
	} else if (CPI_RUN_TESTS == 5) {
		const unsigned char test_ins_assign_lit1[] = { 3, 0, 0x67,  3, 1, 0x22,  3, 2, 0x33,  3, 3, 0x44, };
		vm_mem = (unsigned char *)test_ins_assign_lit1;

		printf("Before: "); vm_dbg_print();

		impl_assign_lit1();
		printf("Pass 0: "); vm_dbg_print();
		impl_assign_lit1();
		printf("Pass 1: "); vm_dbg_print();
		impl_assign_lit1();
		printf("Pass 2: "); vm_dbg_print();
		impl_assign_lit1();
		printf("Pass 3: "); vm_dbg_print();
	}
	return 0;
}
