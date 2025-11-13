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
