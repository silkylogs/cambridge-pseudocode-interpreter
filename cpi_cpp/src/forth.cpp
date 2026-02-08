#include "forth.hpp"

// Helper functions

// Forth word entry: [Link ptr][prim=0,immed,len...][name][header ptr][def]...
// Primitive entry:  [Link ptr][prim=1,immed,len...][name][header ptr][primitive index]
bool entry_is_primitive(Forth::State& self) {
    return self.memory[*self.ptr_instruction + 1] & 0x8000;
}
/*
bool entry_is_immed(Cell* m) {
    return m[inst_ptr + 1] & 0x4000;
}
Cell entry_headerptr(Cell* m) {
    return m[inst_ptr + entry_strlen(m) + 2];
}
*/
Cell entry_strlen(Forth::State& self) {
    return self.memory[*self.ptr_instruction + 1] & 0x3FFF;
}
Cell entry_first_definition(Forth::State& self) {
    return self.memory[*self.ptr_instruction + entry_strlen(self) + 3];
}
Cell entry_primidx(Forth::State& self) {
    return self.memory[*self.ptr_instruction + entry_strlen(self) + 3];
}

// Helper functions end

void Forth::inner_interpreter(State& self) {
    // Push the next addr
    (*self.ptr_return_stack)++;
    self.memory[*self.ptr_return_stack] = *self.ptr_instruction + 1;

    *self.ptr_instruction = self.memory[*self.ptr_instruction]; // Jump to header pointer
    *self.ptr_instruction = self.memory[*self.ptr_instruction]; // Jump to entry header

    // At the entry header,
    if (entry_is_primitive(self)) {         // entry->is_primitive
        Cell primidx = entry_primidx(self); // entry->primitive_idx
        if (primidx >= 0 || primidx < self.primitives.size())
            (self.primitives.at(primidx))(self);
        else std::cout << std::endl
            << "Tried to call a primitive with index " << primidx << " that does not exist."
            << std::endl;

        // Jump to next addr
        *self.ptr_instruction = self.memory[*self.ptr_return_stack];
        (*self.ptr_return_stack)--;
    }
    else {
        // Jump to 1st def
        *self.ptr_instruction = entry_first_definition(self); // inst_ptr += sizeof entry
        *self.ptr_instruction = self.memory[*self.ptr_instruction];
    }
}

void Forth::Util::alloc_set_memory_conents(State& self, char* const byte_string, size_t strlen) {
    self.memory = new Cell[strlen];
    std::copy(
        byte_string,
        byte_string + strlen,
        reinterpret_cast<Byte*>(self.memory));
}

void Forth::Util::fixup_ptrs_after_allocation(State& self) {
    self.ptr_instruction = &self.memory[0];
    self.ptr_data_stack = &self.memory[1];
    self.ptr_return_stack = &self.memory[2];
    self.dict_latest_defined_entry = &self.memory[3];
    self.dict_next_available_cell = &self.memory[4];
}


void Forth::Util::append_def_to_latest_entry(State& self, char* const byte_string, size_t strlen) {
    if (!(strlen % sizeof Cell)) throw std::exception {"Not a multiple."};
    std::copy(
        byte_string,
        byte_string + strlen,
        reinterpret_cast<Byte*>(self.dict_next_available_cell));
    self.dict_next_available_cell += strlen / sizeof Cell;
}


void Forth::Util::add_new_entry(State& self, char* const entry_as_byte_string, size_t bstrlen) {
    Cell old_dict_latest_defined_entry = *self.dict_latest_defined_entry;
    append_def_to_latest_entry(self, entry_as_byte_string, bstrlen);
    *self.dict_latest_defined_entry = old_dict_latest_defined_entry;
    throw std::exception {"todo"};
}

/*
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
    // TODO: High level usage
    //ForthState fs {};
    //fs.addprim("PrintHello", [](ForthState &self) { std::cout << "Hello, world\n"; });
    //volatile auto *mem = fs.get_ptr_to_memory(); // For viewing through a debugger
    //fs.execute("PrintHello");
    

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
*/