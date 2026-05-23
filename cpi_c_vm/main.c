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
 byte vm_mem[(unsigned)1 << 31]; // Whatever a uint32 can address
 word vm_gpr[(unsigned)1 << 7]; // Whatever a byte can address

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

const int INVALID = 0xFFFFFFFF;

int write_src_rmab(byte *adr, Parameters p) {
 int sz = 0;

 sz += write_byte(adr, p.src_rmab);
 if (p.src_rmab == 0) {
  sz += write_byte(adr, p.src_r);
 } else if (p.src_rmab == 1) {
  sz += write_word(adr, p.src_m);
 } else if (p.src_rmab == 2) {
  sz += write_word(adr, p.src_aptr);
  sz += write_word(adr, p.src_alen);
 } else if (p.src_rmab == 3) {
  sz += write_byte(adr, p.src_b);
 } else {
  return INVALID;
 }

 return sz;
}

int write_dst_rmab(byte *adr, Parameters p) {
 int sz = 0;

 sz += write_byte(adr, p.dst_rmab);
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
}

int read_src_rmab(byte *adr, Parameters *p) {
 int sz = 0;
 sz += read_byte(adr, &p->src_rmab);
 if (p->src_rmab == 0) {
  sz += read_byte(adr, &p->src_r);
 } else if (p->src_rmab == 1) {
  sz += read_word(adr, &p->src_m);
 } else if (p->src_rmab == 2) {
  sz += read_word(adr, &p->src_aptr);
  sz += read_word(adr, &p->src_alen);
 } else if (p->src_rmab == 3) {
  sz += read_byte(adr, &p->src_b);
 } else {
  return INVALID;
 }
 return sz;
}

int read_dst_rmab(byte *adr, Parameters *p) {
 int sz = 0;
 sz += read_byte(adr, &p->dst_rmab);
 if (p->dst_rmab == 0) {
  sz += read_byte(adr, &p->dst_r);
 } else if (p->dst_rmab == 1) {
  sz += read_word(adr, &p->dst_m);
 } else if (p->dst_rmab == 2) {
  sz += read_word(adr, &p->dst_aptr);
  sz += read_word(adr, &p->dst_alen);
 } else if (p->dst_rmab == 3) {
  sz += read_byte(adr, &p->dst_b);
 } else {
  return INVALID;
 }
 return sz;
}


// Instructions:
// --------------
// Key:
// (register/memory address/(ptr,sz) array/byte)
// _ = blank or invalid
int write_instr(byte *adr, Parameters p) {
 word sz = 0;

 // Base instruction
 sz += write_byte(adr + sz, p.instr);
 if (p.instr == 0) {
  // Zero trap
  return sz;
 } else if (p.instr == 1) {
  // Assignment: 
  // - r/m/a/_ = op r/m/_/b 
  // - a = a
  // Disallow b = a
  if (p.src_rmab == 3 && p.dst_rmab == 2) return INVALID;
  sz += write_dst_rmab(adr + sz, p);
  sz += write_src_rmab(adr + sz, p);
  return sz;
 } else if (p.instr == 2) {
  // Arithmetic (monadic)
  // - r/m/a/_ = op r/m/_/b 
  // - a = op a
  if (p.src_rmab == 3 && p.dst_rmab == 2) return INVALID;
  sz += write_dst_rmab(adr + sz, p);
  sz += write_byte(adr + sz, p.op);
  sz += write_src_rmab(adr + sz, p);
  return sz;
 } else if (p.instr == 3) {
  // Proc call
  // - r/m/a/_
  if (p.src_rmab == 3) return INVALID;
  sz += write_src_rmab(adr + sz, p);
  return sz;
 } else if (p.instr == 4) {
  // Proc return
  return sz;
 } else if (p.instr == 5) {
  // External proc call: 5
  // - r/m/a/_
  if (p.src_rmab == 3) return INVALID;
  sz += write_src_rmab(adr + sz, p);
  return sz;
 } else {
  return INVALID;
 }
 return sz;
}

int read_instr(byte *adr, Parameters *p) {
 Parameters zeroed = {0};
 word sz = 0;

 *p = zeroed;
 sz += read_byte(adr + sz, &p->instr);
 if (p->instr == 0) {
  return sz;
 } else if (p->instr == 1) {
  if (p->src_rmab == 3 && p->dst_rmab == 2) return INVALID;
  sz += read_dst_rmab(adr + sz, p);
  sz += read_src_rmab(adr + sz, p);
  return sz;
 } else if (p->instr == 2) {
  if (p->src_rmab == 3 && p->dst_rmab == 2) return INVALID;
  sz += read_dst_rmab(adr + sz, p);
  sz += read_byte(adr + sz, &p->op);
  sz += read_src_rmab(adr + sz, p);
  return sz;
 } else if (p->instr == 3) {
  if (p->src_rmab == 3) return INVALID;
  sz += read_src_rmab(adr + sz, p);
  return sz;
 } else if (p->instr == 4) {
  return sz;
 } else if (p->instr == 5) {
  if (p->src_rmab == 3) return INVALID;
  sz += read_src_rmab(adr + sz, p);
  return sz;
 } else {
  return INVALID;
 }
 return sz;
}

void print_instr(Parameters p) {
 printf("Instruction type: %d\n", p.instr);
 if (p.instr == 0) {
  printf(" Zero instruction\n");
 } else if (p.instr == 1) {
  printf(" Assignment:\n");
  printf("  Destination type: %d\n", p.dst_rmab);
  if (p.dst_rmab == 0) {
   printf("   Register: %d\n", p.dst_r);
  } else if (p.dst_rmab == 1) {
   printf("   Memory: %x\n", p.dst_m);
  } else if (p.dst_rmab == 2) {
   printf("   Array: %x,%d\n", p.dst_aptr, p.dst_alen);
  } else if (p.dst_rmab == 3) {
   printf("   Byte: %x\n", p.dst_b);
  } else {
   printf("   Error: Invalid type\n");
  }
  printf("  Source type: %d\n", p.src_rmab);
  if (p.src_rmab == 0) {
   printf("   Register: %d\n", p.src_r);
  } else if (p.src_rmab == 1) {
   printf("   Memory: %x\n", p.src_m);
  } else if (p.src_rmab == 2) {
   printf("   Array: %x,%d\n", p.src_aptr, p.src_alen);
  } else if (p.src_rmab == 3) {
   printf("   Byte: %x\n", p.src_b);
  } else {
   printf("   Error: Invalid type\n");
  }
 } else if (p.instr == 2) {
  printf(" Arithmetic:\n");
  printf("  Destination type: %d\n", p.dst_rmab);
  if (p.dst_rmab == 0) {
   printf("   Register: %d\n", p.dst_r);
  } else if (p.dst_rmab == 1) {
   printf("   Memory: %x\n", p.dst_m);
  } else if (p.dst_rmab == 2) {
   printf("   Array: %x,%d\n", p.dst_aptr, p.dst_alen);
  } else if (p.dst_rmab == 3) {
   printf("   Byte: %x\n", p.dst_b);
  } else {
   printf("   Error: Invalid type\n");
  }
  printf("  Operator: %d\n", p.op);
  {
   char *o[] = { "+", "-", "*", "/" };
   printf("   %s\n", o[p.op]);
  }
  printf("  Source type: %d\n", p.src_rmab);
  if (p.src_rmab == 0) {
   printf("   Register: %d\n", p.src_r);
  } else if (p.src_rmab == 1) {
   printf("   Memory: %x\n", p.src_m);
  } else if (p.src_rmab == 2) {
   printf("   Array: %x,%d\n", p.src_aptr, p.src_alen);
  } else if (p.src_rmab == 3) {
   printf("   Byte: %x\n", p.src_b);
  } else {
   printf("   Error: Invalid type\n");
  }
 } else if (p.instr == 3) {
  printf(" Proc call:\n");
 } else if (p.instr == 4) {
  printf(" Proc return:\n");
 } else if (p.instr == 5) {
  printf(" External proc call:\n");
 } else {
   printf(" Error: Invalid type\n");
 }
}

int main(void) {
#ifdef CPI_RUN_TESTS
 int test_idx = 1;
 for (test_idx = 1; test_idx <= 5; ++test_idx) {
  printf("\n===[test_idx %d]===\n", test_idx);

  if (test_idx == 1) {
   byte mem[1] = { 0 };
   Parameters p = { .instr=0, };
   write_instr(mem, p);
   read_instr(mem, &p);
   print_instr(p);
  } else if (test_idx == 2) {
   byte mem[10] = { 0 };
   Parameters p = { 
    .instr = 1, 
    .dst_rmab = 0,
    .dst_r = 2,
    .src_rmab = 0,
    .src_r = 3,
   };
   write_instr(mem, p);
   read_instr(mem, &p);
   print_instr(p);
  } else if (test_idx == 3) {
  } else if (test_idx == 4) {
  } else if (test_idx == 5) {
  }
 }
#else
 printf("Actually run the program here.\n");
#endif
 return 0;
}

