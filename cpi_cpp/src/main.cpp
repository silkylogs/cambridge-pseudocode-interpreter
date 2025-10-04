#include <iostream>
#include "forth.hpp"

void exit(Forth::State& self) { printf("Exiting...\n"); self.exit_requested = true; };
void nop(Forth::State& self) { (void)self; printf("nop "); };
void dpush(Forth::State& self) { (*self.dstk_ptr)++; }
void dpop(Forth::State& self) { (*self.dstk_ptr)--; }
void rpush(Forth::State& self) { (*self.rstk_ptr)++; }
void rpop(Forth::State& self) { (*self.rstk_ptr)--; }
void lit(Forth::State& self) {
    dpush(self);
    self.memory[*self.dstk_ptr] = self.memory[self.memory[*self.rstk_ptr]];
    self.memory[*self.rstk_ptr]++;
}
void printchar(Forth::State& self) { printf("%c", self.memory[*self.dstk_ptr]); (*self.dstk_ptr)--; }
void brz(Forth::State& self) {
    Cell addr = self.memory[*self.dstk_ptr];
    (*self.dstk_ptr)--;
    Cell cond = self.memory[*self.dstk_ptr];
    (*self.dstk_ptr)--;
    if (!cond) {
        self.memory[*self.rstk_ptr] = addr;
    }
}
void sto(Forth::State& self) {
    Cell addr = self.memory[*self.dstk_ptr];
    (*self.dstk_ptr)--;
    Cell val = self.memory[*self.dstk_ptr];
    (*self.dstk_ptr)--;
    self.memory[addr] = val;
}
void lod_cell(Forth::State& self) {
    Cell addr = self.memory[*self.dstk_ptr];
    self.memory[*self.dstk_ptr] = self.memory[addr];
}
void inc(Forth::State& self) { self.memory[*self.dstk_ptr]++; }

std::vector<void(*)(Forth::State&)> prims {
    exit, nop, dpush, dpop,
    rpush, rpop, lit, printchar,
    brz, sto, lod_cell, inc,
};

int main() {
    /* TODO: High level usage
    ForthState fs {};
    fs.addprim("PrintHello", [](ForthState &self) { std::cout << "Hello, world\n"; });
    volatile auto *mem = fs.get_ptr_to_memory(); // For viewing through a debugger
    fs.execute("PrintHello");
    */

    Forth::State fs{};
    char rom[] =
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
        "\x24\x00"
        ;
    Forth::alloc_set_memory_conents(fs, rom, sizeof(rom));
    Forth::fixup_ptrs_after_allocation(fs);
    //fs.primitives = prims;
    do {
        Forth::inner_interpreter(fs);
    } while (!fs.exit_requested);

    return 0;
}
