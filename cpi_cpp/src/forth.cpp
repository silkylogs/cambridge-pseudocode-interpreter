#include "forth.hpp"

void Forth::alloc_set_memory_conents(State& self, char* const byte_string, size_t strlen) {
    self.memory = new Cell[strlen];
    std::copy(
        byte_string,
        byte_string + strlen,
        reinterpret_cast<Byte*>(self.memory));
}

// Helper functions

// Forth word entry: [Link ptr][prim=0,immed,len...][name][header ptr][def]...
// Primitive entry:  [Link ptr][prim=1,immed,len...][name][header ptr][primitive index]
bool entry_is_primitive(Forth::State& self) {
    return self.memory[*self.inst_ptr + 1] & 0x8000;
}
/*
bool entry_is_immed(Cell* m) {
    return m[inst_ptr + 1] & 0x4000;
}
Cell entry_headerptr(Cell* m) {
    return m[inst_ptr + entry_strlen(m) + 2];
}
*/
Cell entry_strlen(Forth::State &self) {
    return self.memory[*self.inst_ptr + 1] & 0x3FFF;
}
Cell entry_first_definition(Forth::State &self) {
    return self.memory[*self.inst_ptr + entry_strlen(self) + 3];
}
Cell entry_primidx(Forth::State &self) {
    return self.memory[*self.inst_ptr + entry_strlen(self) + 3];
}

// Helper functions end

void Forth::inner_interpreter(State& self) {
    // Push the next addr
    (*self.rstk_ptr)++;
    self.memory[*self.rstk_ptr] = *self.inst_ptr + 1;

    *self.inst_ptr = self.memory[*self.inst_ptr]; // Jump to header pointer
    *self.inst_ptr = self.memory[*self.inst_ptr]; // Jump to entry header

    // At the entry header,
    if (entry_is_primitive(self)) {         // entry->is_primitive
        Cell primidx = entry_primidx(self); // entry->primitive_idx
        try {
            (self.primitives.at(primidx))(self);
        } catch (std::out_of_range& e) {
            std::cout << std::endl
                << "Tried to call a primitive with index " << primidx << " that does not exist." << std::endl
                << "C++ Exception: " << e.what() << std::endl;
            throw std::exception{"Quitting application."};
        };

        // Jump to next addr
        *self.inst_ptr = self.memory[*self.rstk_ptr];
        (*self.rstk_ptr)--;
    }
    else {
        // Jump to 1st def
        *self.inst_ptr = entry_first_definition(self); // inst_ptr += sizeof entry
        *self.inst_ptr = self.memory[*self.inst_ptr];
    }
}

void Forth::fixup_ptrs_after_allocation(State &self) {
    self.inst_ptr = &self.memory[0];
    self.dstk_ptr = &self.memory[1];
    self.rstk_ptr = &self.memory[2];
}
