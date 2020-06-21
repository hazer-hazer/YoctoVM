#include "backend/VM.h"

VM::VM(){
	reset_stack();
}

VM::~VM(){

}

// Chunk
Value VM::read_const(){
	return chunk->constants.values[read_byte()];
}

uint8_t VM::read_byte(){
	// Read next byte
	return *ip++;
}

// Stack
void VM::reset_stack(){
	stack_top = stack;
}

void VM::push(Value val){
	*stack_top = value;
	stack_top++;
}

Value VM::pop(){
	stack_top--;
	return *stack_top;
}

// Interpretation
InterpretResult VM::interpret(Chunk * chunk){
	this->chunk = chunk;
	ip = chunk->code;
	return run();
}

InterpretResult VM::run(){
	for(;;){
		switch(read_byte()){
			case OP_CONST:{
				Value constant = read_const();
				break;
			}
			case OP_RETURN:{
				return INTERPRET_OK;
			}
		}
	}
}