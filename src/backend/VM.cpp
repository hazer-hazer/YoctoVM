#include "backend/VM.h"

VM::VM(){}

// Chunk
Value VM::read_const(){
	return chunk->constants.values[read_byte()];
}

uint8_t VM::read_byte(){
	return chunk->code[current++];
}

// Stack
void VM::push(Value val){
	stack.push(val);
}

Value VM::pop(){
	Value top = stack.top();
	stack.pop();
	return top;	
}

// Interpretation
InterpretResult VM::interpret(Chunk * chunk){
	this->chunk = chunk;
	current = 0;
	return run();
}

InterpretResult VM::run(){
	for(;;){
		switch(read_byte()){
			case OP_CONST:{
				Value constant = read_const();
				push(constant);
				break;
			}
			case OP_NEGATE:{
				push(-pop());
				break;
			}
			case OP_RETURN:{
				std::cout << pop();
				return INTERPRET_OK;
			}
		}
	}
}