#include "forth.hpp"

Cell *ForthState::backing_memory() {
	auto &self = *this;
	return self.memory.data();
}

void ForthState::overwrite_memory(char *const byte_string, size_t strlen) {
	auto &self = *this;
	self.memory.resize(strlen / sizeof (Cell));
	std::copy(
		byte_string, 
		byte_string + strlen,
	    reinterpret_cast<char*>(self.memory.data()));
}

void ForthState::run() {
	std::cout << "TODO: run\n";
}
