#ifndef VM_H
#define VM_H

#include <iostream>
#include <stack>
#include "backend/Chunk.h"

typedef enum {
	INTERPRET_OK,
	INTERPRET_COMPILE_ERROR,
	INTERPRET_RUNTIME_ERROR
} InterpretResult;

class VM {
public:
	VM();
	virtual ~VM() = default;

	InterpretResult interpret(Chunk * chunk);
	InterpretResult run();

private:
	// Chunk
	Chunk * chunk;
	size_t current;
	uint8_t read_byte();
	Value read_const();

	// Stack
	std::stack<Value> stack;
	void push(Value val);
	Value pop();
};

#endif