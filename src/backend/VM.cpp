#include "backend/VM.h"

VM::VM(Chunk & chunk) : chunk(chunk) {
	ip = 0;
	stack.clear();
}

InterpretResult VM::run(){
	std::cout << "Run bytecode...\n";

	while(true){

		auto instruction = OpCode(read_byte());
		switch(instruction){
			case OpCode::CONSTANT:{
				auto constant = read_const();
				push(constant);
				break;
			}
			case OpCode::CONST_NULL:{
				push(std::monostate());
				break;
			}
			case OpCode::POP:{
				pop();
				break;
			}
			case OpCode::GET_GLOBAL:{
				auto name = read_string();
				auto found = globals.find(name);
				if(found == globals.end()){
					runtime_error(name + " is not defined");
				}
				auto value = found->second;
				push(value);
				break;
			}
			case OpCode::DEFINE_GLOBAL:{
				auto name = read_string();
				globals[name] = peek(0);
				pop();
				break;
			}
			case OpCode::SET_GLOBAL:{
				auto name = read_byte();
				auto found = globals.find(name);
				if(found == globals.end()){
					runtime_error(name + " is not defined");
				}
				found->second = peek(0);
				break;
			}
			case OpCode::SET_LOCAL:{
				uint8_t slot = read_byte();
				stack[slot] = peek(0);
				break;
			}
			case OpCode::GET_LOCAL:{
				uint8_t slot = read_byte();
				push(stack[slot]);
				break;
			}
			case OpCode::JUMP:{
				auto offset = read_short();
				ip += offset;
				break;
			}
			// case OpCode::JUMP_IF_FALSE:{
			// 	auto offset = read_short();
			// 	// if()
			// }
		}
	}
}