#include "backend/VM.h"

VM::VM(Function * root){
	stack.clear();
	frames.clear();

	// Push root function
	push({ Function, root });
	frame->function = root;
	frame->ip = 0;
	frame->slots = stack;
}

void VM::run(){
	std::cout << "Run bytecode...\n";

	CallFrame * frame = &frames.back();

	while(true){

		auto instruction = OpCode(read_byte());
		switch(instruction){
			case OpCode::Const:{
				auto constant = read_const();
				push(constant);
				break;
			}
			case OpCode::Null:{
				push({ValueType::Null, nullptr});
				break;
			}
			case OpCode::Pop:{
				pop();
				break;
			}
			case OpCode::GetGlobal:{
				auto name = read_string();
				auto found = globals.find(name);
				if(found == globals.end()){
					runtime_error(name + " is not defined");
				}
				auto value = found->second;
				push(value);
				break;
			}
			case OpCode::DefineGlobal:{
				auto name = read_string();
				globals[name] = peek(0);
				pop();
				break;
			}
			case OpCode::SetGlobal:{
				auto name = read_string();
				auto found = globals.find(name);
				if(found == globals.end()){
					runtime_error(name + " is not defined");
				}
				found->second = peek(0);
				break;
			}
			case OpCode::SetLocal:{
				uint8_t slot = read_byte();
				frame->slots[slot] = peek(0);
				break;
			}
			case OpCode::GetLocal:{
				uint8_t slot = read_byte();
				push(frame->slots[slot]);
				break;
			}
			case OpCode::Jump:{
				auto offset = read_short();
				frame->ip += offset;
				break;
			}
			case OpCode::JumpIfFalse:{
				auto offset = read_short();
				// If falsy -> jump
				if(!peek_bool(0)){
					frame->ip += offset;
				}
				break;
			}
			case OpCode::Print:{
				Value val = pop();
				DataObject * dataObj = dynamic_cast<DataObject*>(val.obj);
				if(dataObj){
					std::cout << dataObj->toString()->get_val() << std::endl;
				}else{
					std::cout << "Object[" << val.obj << "]" << std::endl;
				}
				break;
			}
			case OpCode::StopCode:{
				return;
			}
		}
	}
}