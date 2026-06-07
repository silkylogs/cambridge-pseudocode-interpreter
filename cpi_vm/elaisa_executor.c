#include "elaisa_platform_boilerplate.c"
#include "elaisa_utils.c"

typedef void (*Fn)(void*);

void call_fn_ptr_idx(Fn *fns, word idx) {
	(fns + idx)();
}

struct Rmab {
 byte rmab_tag;
 byte rmab_r_reg;
 word rmab_m_mem;
 word rmab_a_ptr, rmab_a_len;
 byte rmab_b_byte;
};


void print_rmab_human(struct Rmab r) {
 if (r.rmab_tag > 3) printf("!!! ");
 if (r.rmab_tag == 0) printf("GPR %2.2x ", r.rmab_r_reg);
 else if (r.rmab_tag == 1) printf("MEM 0x%8.8x ", r.rmab_m_mem);
 else if (r.rmab_tag == 2) printf("ARR 0x%x 0x%8.8x ", r.rmab_a_ptr, r.rmab_a_len);
 else if (r.rmab_tag == 3) printf("BYT 0x%2.2x ", r.rmab_b_byte);
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

 printf("Vm { \n");
 printf("\tRIP = 0x%8.8x\n", v->vm_rip);
 printf("\tRFLAGS = 0x%8.8x\n", v->vm_rflags);
 for (i=0; i<GPR_COUNT; ++i) printf("\tGPR %2.2X = 0x%8.8x\n", i, v->vm_gpr[i]);
 printf("\tHost exception callback @ 0x%p\n", (void*)v->vm_exception_callback);
 printf("}\n");
}

void vm_default_exception_callback(Vm *v, Instr p, const char *msg) {
 printf("\nException \"%s\" raised while processing instruction:\n", msg);
 print_instr_bytes(p); 
 print_instr_human(p);
 printf("Vm state:\n"); 
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
  if (p.param_dst.rmab_tag == 0) {
   if (p.param_src.rmab_tag == 0) {
    v->vm_gpr[p.param_dst.rmab_r_reg] = v->vm_gpr[p.param_src.rmab_r_reg];
   } else if (p.param_src.rmab_tag == 1) {
    word src;
    byte *adr = &v->vm_mem[p.param_src.rmab_m_mem];
    read_word(adr, &src);
    v->vm_gpr[p.param_dst.rmab_r_reg] = src;
   } else if (p.param_src.rmab_tag == 2) {
    v->vm_exception_callback(v, p, "Illegal instruction. Assigning array to register.");
   } else if (p.param_src.rmab_tag == 3) {
    v->vm_gpr[p.param_dst.rmab_r_reg] = p.param_src.rmab_b_byte;
   } else {
    v->vm_exception_callback(v, p, "Illegal instruction. Destination object tag out of range.");
   }
  } else if (p.param_dst.rmab_tag == 1) {
   if (p.param_src.rmab_tag == 0) {
    word reg = v->vm_gpr[p.param_src.rmab_r_reg];
    byte *dst = &v->vm_mem[p.param_dst.rmab_m_mem];
    write_word(dst, reg);
   } else if (p.param_src.rmab_tag == 1) {
    word tmp;
    byte *dst = &v->vm_mem[p.param_dst.rmab_m_mem];
    byte *src = &v->vm_mem[p.param_src.rmab_m_mem];
    read_word(src, &tmp);
    write_word(dst, tmp);
   } else if (p.param_src.rmab_tag == 2) {
    v->vm_exception_callback(v, p, "Illegal instruction. Assigning array to memory address.");
   } else if (p.param_src.rmab_tag == 3) {
    byte *dst = &v->vm_mem[p.param_dst.rmab_m_mem];
    write_byte(dst, p.param_src.rmab_b_byte);
   } else {
    v->vm_exception_callback(v, p, "Illegal instruction. Destination object tag out of range.");
   }
  } else if (p.param_dst.rmab_tag == 2) {
   if (p.param_src.rmab_tag == 0) {
    v->vm_exception_callback(v, p, "Illegal instruction. Unimplemented.");
   } else if (p.param_src.rmab_tag == 1) {
    v->vm_exception_callback(v, p, "Illegal instruction. Unimplemented.");
   } else if (p.param_src.rmab_tag == 2) {
    v->vm_exception_callback(v, p, "Illegal instruction. Unimplemented.");
   } else if (p.param_src.rmab_tag == 3) {
    v->vm_exception_callback(v, p, "Illegal instruction. Unimplemented.");
   } else {
    v->vm_exception_callback(v, p, "Illegal instruction. Destination object tag out of range.");
   }
  } else if (p.param_dst.rmab_tag == 3) {
   if (p.param_src.rmab_tag == 0) {
    v->vm_exception_callback(v, p, "Illegal instruction. Unimplemented.");
   } else if (p.param_src.rmab_tag == 1) {
    v->vm_exception_callback(v, p, "Illegal instruction. Unimplemented.");
   } else if (p.param_src.rmab_tag == 2) {
    v->vm_exception_callback(v, p, "Illegal instruction. Unimplemented.");
   } else if (p.param_src.rmab_tag == 3) {
    v->vm_exception_callback(v, p, "Illegal instruction. Unimplemented.");
   } else {
    v->vm_exception_callback(v, p, "Illegal instruction. Destination object tag out of range.");
   }
  } else {
    v->vm_exception_callback(v, p, "Illegal instruction. Source object tag out of range.");
  }
 } else if (p.param_instr == 2) {
  if (p.param_dst.rmab_tag == 3 && p.param_src.rmab_tag == 2) {
   v->vm_exception_callback(v, p, "Illegal instruction. Assigning array to byte literal.");
  }
 } else if (p.param_instr == 3) {
    v->vm_exception_callback(v, p, "Illegal instruction. Unimplemented.");
 } else if (p.param_instr == 4) {
    v->vm_exception_callback(v, p, "Illegal instruction. Unimplemented.");
 } else if (p.param_instr == 5) {
    v->vm_exception_callback(v, p, "Illegal instruction. Unimplemented.");
 } else {
   v->vm_exception_callback(v, p, "Illegal instruction. Base instruction tag out of range.");
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
   {
    Vm v = { 0 };
    byte mem[1] = { 0 };
    Instr p = { 0 };

    v.vm_mem = mem;
    v.vm_exception_callback = vm_default_exception_callback;

    read_instr(mem, &p);

    print_instr_bytes(p); 
    printf("\n");
    print_instr_human(p);

    printf("Before:\n");
    print_vm(&v);
    vm_exec_instr(&v, p);
    printf("After:\n");
    print_vm(&v);
   }
  } else if (test_idx == 3) {
   {
    Vm v = { 0 };
    byte mem[5] = { 1, 0, 12, 0, 15 };
    Instr p = { 0 };

    v.vm_mem = mem;
    v.vm_exception_callback = vm_default_exception_callback;
    v.vm_gpr[15] = 0x224488CC;
    v.vm_gpr[12] = 0x44;

    read_instr(mem, &p);

    print_instr_bytes(p); 
    printf("\n");
    print_instr_human(p);

    printf("Before:\n");
    print_vm(&v);
    vm_exec_instr(&v, p);
    printf("After:\n");
    print_vm(&v);
   }
  } else if (test_idx == 4) {
  }
 }
#else
 printf("Actually run the program here.\n");
#endif
 return 0;
}

