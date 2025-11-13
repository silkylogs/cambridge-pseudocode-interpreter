#include <iostream>
#include "forth.hpp"

void exit(Forth::State& self) {
    printf("Exiting...\n");
    self.exit_requested = true;
};
void nop(Forth::State& self) {
    (void)self;
    printf("nop ");
};
void dpush(Forth::State& self) {
    (*self.ptr_data_stack)++;
}
void dpop(Forth::State& self) {
    (*self.ptr_data_stack)--;
}
void rpush(Forth::State& self) {
    (*self.ptr_return_stack)++;
}
void rpop(Forth::State& self) {
    (*self.ptr_return_stack)--;
}

// Pushes the cell next to that instruction to the stack, then adjusts the return accordingly
void lit(Forth::State& self) {
    auto literal_value = self.memory[self.memory[*self.ptr_return_stack]];

    // Push literal value
    (*self.ptr_data_stack)++;
    self.memory[*self.ptr_data_stack] = literal_value;

    // Dont return to the literal, return to the addr one beyond it.
    self.memory[*self.ptr_return_stack]++;
}
void printchar(Forth::State& self) {
    printf("%c", self.memory[*self.ptr_data_stack]); (*self.ptr_data_stack)--;
}
void brz(Forth::State& self) {
    Cell addr = self.memory[*self.ptr_data_stack];
    (*self.ptr_data_stack)--;
    Cell cond = self.memory[*self.ptr_data_stack];
    (*self.ptr_data_stack)--;
    if (!cond) {
        self.memory[*self.ptr_return_stack] = addr;
    }
}
void sto(Forth::State& self) {
    Cell addr = self.memory[*self.ptr_data_stack];
    (*self.ptr_data_stack)--;
    Cell val = self.memory[*self.ptr_data_stack];
    (*self.ptr_data_stack)--;
    self.memory[addr] = val;
}
void lod_cell(Forth::State& self) {
    Cell addr = self.memory[*self.ptr_data_stack];
    self.memory[*self.ptr_data_stack] = self.memory[addr];
}
void inc(Forth::State& self) { self.memory[*self.ptr_data_stack]++; }

std::vector<void(*)(Forth::State&)> prims{
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
    Forth::Util::alloc_set_memory_conents(fs, rom, sizeof(rom));
    Forth::Util::fixup_ptrs_after_allocation(fs);
    fs.primitives = prims;
    do {
        Forth::inner_interpreter(fs);
    } while (!fs.exit_requested);

    return 0;
}
