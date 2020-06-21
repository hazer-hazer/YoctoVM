#ifndef CHUNK_H
#define CHUNK_H

#include <iostream>
#include <iomanip>

#include "backend/Value.h"

typedef enum {
	OP_CONST,
	OP_RETURN
} OpCode;

struct Chunk {
	int count;
	int capacity;
	uint8_t * code;
	int * lines;

	ValueArray constants;

	Chunk();
	virtual ~Chunk() = default;

	void init();
	void clear();
	void write(uint8_t byte, int line);
	int add_const(Value val);

	// Debug
	void disasm();
	int disasm_instruction(int offset);
	int disasm_const(int offset);
};

#endif