#ifndef VM_H
#define VM_H

#include <iostream>
#include <vector>
#include <unordered_map>

#include "Exceptions.h"
#include "backend/Chunk.h"

enum InterpretResult {
	OK,
	COMPILE_ERROR,
	RUNTIME_ERROR
};

class VM {
public:
	VM();
	virtual ~VM() = default;

	InterpretResult run();

private:
	// Chunk
	Chunk & chunk;
	unsigned int ip;

	std::vector<Value> stack;
	std::unordered_map<std::string, Value> globals;

	inline void reset_stack(){
		stack.clear();
	}

	inline void push(Value val){
		stack.push_back(val);
	}
	inline Value pop(){
		auto v = stack.back();
		stack.pop_back();
		return v;
	}
	inline Value peek(int distance){
		return stack[stack.size() - 1 - distance];
	}

	// Chunk manips
	inline uint8_t read_byte(){
		return chunk.read_byte(ip++);
	}
	inline Value read_const(){
		return chunk.read_const(read_byte());
	}
	inline uint16_t read_short(){
		// Just read two bytes and disjunct them
		ip++;
		return ((chunk.get_byte(ip - 2) << 8) | (chunk.get_byte(ip - 1)));
	}
	inline std::string read_string(){
		return std::get<std::string>(read_const());
	}

	// Errors
	inline void runtime_error(const std::string & msg){
		throw YoctoException(msg);
	}

};

#endif