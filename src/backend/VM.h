#ifndef VM_H
#define VM_H

#include "backend/Chunk.h"

#define STACK_MAX 256

typedef enum {
	INTERPRET_OK,
	INTERPRET_COMPILE_ERROR,
	INTERPRET_RUNTIME_ERROR
} InterpretResult;

class VM {
public:
	VM();
	~VM();

	InterpretResult interpret(Chunk * chunk);
	InterpretResult run();

private:
	// Chunk
	Chunk * chunk;
	uint8_t * ip;
	uint8_t read_byte();
	Value read_const();

	// Stack
	Value stack[STACK_MAX];
	Value * stack_top;
	void reset_stack();
	void push(Value val);
	Value pop();
};

#endif