#if defined PLATFORM_APPLE
 int printf(const char *, ...);
 void *memcpy(void *dst, const void *src, unsigned long sz);
 void exit(int);
 #define CHAR_BIT 8
#elif defined PLATFORM_WINDOWS
 int printf(const char *, ...);
 void *memcpy(void *dst, const void *src, unsigned long long sz);
 void exit(int);
 #define CHAR_BIT 8
#else 
 int printf(const char *, ...);
 void *memcpy(void *dst, const void *src, unsigned long long sz);
 void exit(int);
 #define CHAR_BIT 8
#endif

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

struct Rmab {
 byte rmab_tag;
 byte rmab_r_reg;
 word rmab_m_mem;
 word rmab_a_ptr, rmab_a_len;
 byte rmab_b_byte;
};

void print_byte(byte b) {
 printf("%2.2x ", b);
}

void print_rmab_human(struct Rmab r) {
 if (r.rmab_tag > 3) printf("!!! ");
 if (r.rmab_tag == 0) printf("GPR %2.2x ", r.rmab_r_reg);
 else if (r.rmab_tag == 1) printf("MEM 0x%8.8x ", r.rmab_m_mem);
 else if (r.rmab_tag == 2) printf("ARR 0x%x 0x%8.8x ", r.rmab_a_ptr, r.rmab_a_len);
 else if (r.rmab_tag == 3) printf("BYT 0x%2.2x ", r.rmab_b_byte);
}


void print_word_bytes(word i) {
  byte tmp[4] = { 0 };
  memcpy(tmp, &i, sizeof i);
  for (i = 0; i < sizeof i; ++i) {
   print_byte(tmp[i]);
  }
}

void print_rmab_bytes(struct Rmab r) {
 if (r.rmab_tag > 3) {
  printf("!! ");
  return;
 }
 print_byte(r.rmab_tag);
 if (r.rmab_tag == 0) {
  print_byte(r.rmab_r_reg);
 } else if (r.rmab_tag == 1) {
  print_word_bytes(r.rmab_m_mem);
 } else if (r.rmab_tag == 2) {
  print_word_bytes(r.rmab_a_ptr);
  print_word_bytes(r.rmab_a_len);
 } else if (r.rmab_tag == 3) {
  print_byte(r.rmab_b_byte);
 }
}

const int INVALID = 0xFFFFFFFF;

int write_rmab(byte *adr, struct Rmab r) {
 byte *start = adr;

 adr += write_byte(adr, r.rmab_tag);
 if (r.rmab_tag == 0) {
  adr += write_byte(adr, r.rmab_r_reg);
 } else if (r.rmab_tag == 1) {
  adr += write_word(adr, r.rmab_m_mem);
 } else if (r.rmab_tag == 2) {
  adr += write_word(adr, r.rmab_a_ptr);
  adr += write_word(adr, r.rmab_a_len);
 } else if (r.rmab_tag == 3) {
  adr += write_byte(adr, r.rmab_b_byte);
 } else {
  return INVALID;
 }
 return adr - start;
}

int read_rmab(byte *adr, struct Rmab *r) {
 byte *start = adr;

 adr += read_byte(adr, &r->rmab_tag);
 if (r->rmab_tag == 0) {
  adr += read_byte(adr, &r->rmab_r_reg);
 } else if (r->rmab_tag == 1) {
  adr += read_word(adr, &r->rmab_m_mem);
 } else if (r->rmab_tag == 2) {
  adr += read_word(adr, &r->rmab_a_ptr);
  adr += read_word(adr, &r->rmab_a_len);
 } else if (r->rmab_tag == 3) {
  adr += read_byte(adr, &r->rmab_b_byte);
 } else {
  return INVALID;
 }
 return adr - start;
}

typedef struct Instr Instr;
struct Instr {
 byte param_instr;
 struct Rmab param_src, param_dst;
 byte param_op;
};

void print_instr_human(Instr p) {
 if (p.param_instr == 0) {
  printf("ZTRAP");
 } else if (p.param_instr == 1) {
  printf("ASSGN ");
  print_rmab_human(p.param_dst);
  print_rmab_human(p.param_src);
 } else if (p.param_instr == 2) {
  printf("ARITH ");
  print_rmab_human(p.param_dst);
  {
   char *o[] = { "+=", "-=", "*=", "/=" };
   printf("%s ", p.param_op <= 3 ? o[p.param_op] : "!!");
  }
  print_rmab_human(p.param_src);
 } else if (p.param_instr == 3) {
  printf("CALL ");
  print_rmab_human(p.param_src);
 } else if (p.param_instr == 4) {
  printf("RET");
 } else if (p.param_instr == 5) {
  printf("ECALL ");
  print_rmab_human(p.param_src);
 } else printf("!!!");
 printf("\n");
}

void print_instr_bytes(Instr p) {
 if (p.param_instr > 5) { printf("!! "); return; }
 print_byte(p.param_instr);
 if (p.param_instr == 0) {
  ;
 } else if (p.param_instr == 1) {
  print_rmab_bytes(p.param_dst);
  print_rmab_bytes(p.param_src);
 } else if (p.param_instr == 2) {
  print_rmab_bytes(p.param_dst);
  print_byte(p.param_op);
  print_rmab_bytes(p.param_src);
 } else if (p.param_instr == 3) {
  print_rmab_bytes(p.param_src);
 } else if (p.param_instr == 4) {
  ;
 } else if (p.param_instr == 5) {
  print_rmab_bytes(p.param_src);
 } else printf("!! ");
}

int write_instr(byte *adr, Instr p) {
 byte *start = adr;
 
 adr += write_byte(adr, p.param_instr);
 if (p.param_instr == 0) {
  ;
 } else if (p.param_instr == 1) {
  if (p.param_src.rmab_tag == 3 && p.param_dst.rmab_tag == 2) return INVALID;
  adr += write_rmab(adr, p.param_dst);
  adr += write_rmab(adr, p.param_src);
 } else if (p.param_instr == 2) {
  if (p.param_src.rmab_tag == 3 && p.param_dst.rmab_tag == 2) return INVALID;
  adr += write_rmab(adr, p.param_dst);
  adr += write_byte(adr, p.param_op);
  adr += write_rmab(adr, p.param_src);
 } else if (p.param_instr == 3) {
  if (p.param_src.rmab_tag == 3) return INVALID;
  adr += write_rmab(adr, p.param_src);
 } else if (p.param_instr == 4) {
  ;
 } else if (p.param_instr == 5) {
  if (p.param_src.rmab_tag == 3) return INVALID;
  adr += write_rmab(adr, p.param_src);
 } else {
  return INVALID;
 }
 return adr - start;
}

int read_instr(byte *adr, Instr *p) {
 byte *start = adr;
 
 adr += read_byte(adr, &p->param_instr);
 if (p->param_instr == 0) {
  ;
 } else if (p->param_instr == 1) {
  if (p->param_src.rmab_tag == 3 && p->param_dst.rmab_tag == 2) return INVALID;
  adr += read_rmab(adr, &p->param_dst);
  adr += read_rmab(adr, &p->param_src);
 } else if (p->param_instr == 2) {
  if (p->param_src.rmab_tag == 3 && p->param_dst.rmab_tag == 2) return INVALID;
  adr += read_rmab(adr, &p->param_dst);
  adr += read_byte(adr, &p->param_op);
  adr += read_rmab(adr, &p->param_src);
 } else if (p->param_instr == 3) {
  if (p->param_src.rmab_tag == 3) return INVALID;
  adr += read_rmab(adr, &p->param_src);
 } else if (p->param_instr == 4) {
  ;
 } else if (p->param_instr == 5) {
  if (p->param_src.rmab_tag == 3) return INVALID;
  adr += read_rmab(adr, &p->param_src);
 } else {
  return INVALID;
 }
 return adr - start;
}

#define GPR_COUNT 16
typedef struct Vm Vm;
struct Vm {
 byte *vm_mem;
 word vm_gpr[GPR_COUNT]; // Whatever a byte can address

 word vm_rip;
 word vm_rflags;

 void (*vm_exception_callback)(Vm *, Instr p, const char*);
};

void print_vm(Vm *v) {
 int i = 0;

 printf("\nVm { \n");
 printf("\tRIP = 0x%8.8x\n", v->vm_rip);
 printf("\tRFLAGS = 0x%8.8x\n", v->vm_rflags);
 for (i=0; i<GPR_COUNT; ++i) printf("\tGPR %X = %8.8x\n", i, v->vm_gpr[i]);
 printf("\texception callback @ %p\n", (void*)v->vm_exception_callback);
 printf("}\n");
}

void vm_default_exception_callback(Vm *v, Instr p, const char *msg) {
 printf("\nException raised. \"%s\"\n", msg);
 printf("\nProcessing instruction:\n");
 print_instr_human(p);
 printf("\nVm state:\n"); 
 print_vm(v);
 //exit(1);
}

void vm_exec_instr(Vm *v, Instr p) {
 if (p.param_instr == 0) {
  v->vm_exception_callback(v, p, "Zero trap");
 } else if (p.param_instr == 1) {
  if (p.param_dst.rmab_tag == 3 && p.param_src.rmab_tag == 2) {
   v->vm_exception_callback(v, p, "Illegal instruction. Assigning array to byte literal.");
  }

  if (p.dst.rmab_tag == 0) {
   if (p.src.rmab_tag == 0) {
    vm_gpr[p.dst.rmab_r_reg] = vm_gpr[p.src.rmab_r_reg];
   } else if (p.src.rmab_tag == 1) {
    word deref = vm_gpr[p.src.rmab_m_mem];
    vm_gpr[p.dst.rmab_r_reg] = deref;
   } else if (p.src.rmab_tag == 2) {
    v->vm_exception_callback(v, p, "Illegal instruction. Assigning array to register.");
   } else if (p.src.rmab_tag == 3) {
    vm_gpr[p.dst.rmab_r_reg] = p.src.rmab_b_byte;
   } else {
    v->vm_exception_callback(v, p, "Illegal instruction. Object tag out of range.");
   }
  }

 } else if (p.param_instr == 2) {
  if (p.param_dst.rmab_tag == 3 && p.param_src.rmab_tag == 2) {
   v->vm_exception_callback(v, p, "Illegal instruction. Assigning array to byte literal.");
  }
 } else if (p.param_instr == 3) {
 } else if (p.param_instr == 4) {
 } else if (p.param_instr == 5) {
 } else {
   v->vm_exception_callback(v, p, "Illegal instruction. Base out of range.");
 }
}

int main(void) {
#if defined CPI_RUN_TESTS
 int test_idx;
 for (test_idx = 0; test_idx <= 5; ++test_idx) {
  printf("\n===[test_idx %d]===\n", test_idx);

  if (test_idx == 0) {
   {
    struct Rmab r = {
     .rmab_tag = 0,
     .rmab_r_reg = 1,
    };
    print_rmab_bytes(r);
    printf("\n");
    print_rmab_human(r);
    printf("\n");
   }
   {
    struct Rmab r = {
     .rmab_tag = 0,
     .rmab_r_reg = 255,
    };
    print_rmab_bytes(r);
    printf("\n");
    print_rmab_human(r);
    printf("\n");
   }
   {
    struct Rmab r = {
     .rmab_tag = 1,
     .rmab_m_mem = 0xDEADBEEF,
    };
    print_rmab_bytes(r);
    printf("\n");
    print_rmab_human(r);
    printf("\n");
   }
   {
    struct Rmab r = {
     .rmab_tag = 2,
     .rmab_a_ptr = 0xDEADBEEF,
     .rmab_a_len = 0xEFBEADDE,
    };
    print_rmab_bytes(r);
    printf("\n");
    print_rmab_human(r);
    printf("\n");
   }
   {
    struct Rmab r = {
     .rmab_tag = 3,
     .rmab_b_byte = 0xAD,
    };
    print_rmab_bytes(r);
    printf("\n");
    print_rmab_human(r);
    printf("\n");
   }
   {
    struct Rmab r = {
     .rmab_tag = 10,
    };
    print_rmab_bytes(r);
    printf("\n");
    print_rmab_human(r);
    printf("\n");
   }
  } else if (test_idx == 1) {
   {
    Instr p = { 
     .param_instr=0, 
    };
    print_instr_bytes(p); 
    printf("\n");
    print_instr_human(p);
   }
   {
    Instr p = { 
     .param_instr = 1, 
     .param_dst = { .rmab_tag = 0, .rmab_r_reg = 2 },
     .param_src = { .rmab_tag = 0, .rmab_r_reg = 1 },
    };
    print_instr_bytes(p);
    printf("\n");
    print_instr_human(p);
   }
   {
    Instr p = { 
     .param_instr = 2, 
     .param_dst = { .rmab_tag = 2, .rmab_a_ptr = 0x10001000, .rmab_a_len=0x20 },
     .param_op = 1,
     .param_src = { .rmab_tag = 0, .rmab_r_reg = 200 },
    };
    print_instr_bytes(p);
    printf("\n");
    print_instr_human(p);
   }
   {
    Instr p = { 
     .param_instr = 3, 
     .param_dst = { .rmab_tag = 2, .rmab_a_ptr = 0x10001000, .rmab_a_len=0x20 },
     .param_op = 1,
     .param_src = { .rmab_tag = 0, .rmab_r_reg = 200 },
    };
    print_instr_bytes(p);
    printf("\n");
    print_instr_human(p);
   }
   {
    Instr p = { 
     .param_instr = 4, 
    };
    print_instr_bytes(p);
    printf("\n");
    print_instr_human(p);
   }
   {
    Instr p = { 
     .param_instr = 5, 
     .param_src = { .rmab_tag = 3, .rmab_b_byte = 0x46 },
    };
    print_instr_bytes(p);
    printf("\n");
    print_instr_human(p);
   }
   {
    Instr p = { 
     .param_instr = 6, 
    };
    print_instr_bytes(p);
    printf("\n");
    print_instr_human(p);
   }
  } else if (test_idx == 2) {
  } else if (test_idx == 3) {
  } else if (test_idx == 4) {
  }
 }
#else
 printf("Actually run the program here.\n");
#endif
 return 0;
}

