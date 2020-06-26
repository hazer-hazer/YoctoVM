#ifndef CHUNK_H
#define CHUNK_H

#include <iostream>
#include <iomanip>
#include <vector>

#include "backend/Value.h"

enum class OpCode: uint8_t {
	CONST_NULL,
	CONSTANT,

	POP,
	SETUP_LOOP,

	DEFINE_GLOBAL,
	GET_GLOBAL,
	SET_GLOBAL,
	GET_LOCAL,
	SET_LOCAL,

	JUMP,
	JUMP_IF_FALSE,

	// OP_NEGATE,
	// OP_ADD,
	// OP_SUB,
	// OP_MUL,
	// OP_DIV,
	
	// Replace PRINT OpCode with built-in function or use it for REPL
	PRINT,
	
	RETURN
};

struct Chunk {
	std::vector<uint8_t> code;
	std::vector<Value> constants;

	Chunk();
	virtual ~Chunk() = default;

	uint8_t get_byte(int offset) const {
		return code[offset];
	}
	void set_byte(int offset, uint8_t value) {
		code[offset] = value;
	}
	Value get_const(int constant) const {
		return constants[constant];
	}

	void write(uint8_t byte);
	void write(OpCode opcode);
	size_t add_const(Value val);
	int count() {
		return static_cast<int>(code.size());
	}

	// Debug
	void disasm(const std::string & chunk_name);
	int disasm_instruction(int offset);
};

#endif