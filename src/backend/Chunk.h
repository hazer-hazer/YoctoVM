#ifndef CHUNK_H
#define CHUNK_H

#include <iostream>
#include <iomanip>
#include <vector>

#include "backend/Value.h"

typedef enum {
	OP_CONST,
	OP_RETURN,
	OP_NEGATE,
	OP_ADD,
	OP_SUB,
	OP_MUL,
	OP_DIV
} OpCode;

struct Chunk {
	std::vector<uint8_t> code;
	std::vector<int> lines;

	ValueArray constants;

	Chunk();
	virtual ~Chunk() = default;

	void write(uint8_t byte, int line);
	int add_const(Value val);

	// Debug
	void disasm();
	int disasm_instruction(int offset);
	int disasm_const(int offset);
};

#endif