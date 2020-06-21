#include "backend/Chunk.h"
#include "backend/memory.h"

Chunk::Chunk(){
	init();
}

void Chunk::clear(){
	FREE_ARRAY(uint8_t, code, capacity);
	FREE_ARRAY(int, lines, capacity);
	init();
	constants.clear();
}

void Chunk::init(){
	count = 0;
	capacity = 0;
	code = NULL;
}

void Chunk::write(uint8_t byte, int line){
	if(capacity < count + 1){
		int oldCapacity = capacity;
		capacity = GROW_CAPACITY(oldCapacity);
		code = GROW_ARRAY(code, uint8_t, oldCapacity, capacity);
		lines = GROW_ARRAY(lines, int, oldCapacity, capacity);
	}
	code[count] = byte;
	lines[count] = line;
	count++;
}

int Chunk::add_const(Value val){
	constants.write(val);
	return constants.count - 1;
}

///////////
// Debug //
///////////
void Chunk::disasm(){
	for(int offset = 0; offset < count;){
		offset = disasm_instruction(offset);
	}
}

int Chunk::disasm_instruction(int offset){
	std::cout << std::setfill('0') << std::setw(4) << offset << " ";

	if(offset > 0 && lines[offset] == lines[offset - 1]){
		std::cout << "   |";
	}else{
		std::cout << std::setfill('0') << std::setw(4) << lines[offset];
	}

	std::cout << ' ';

	uint8_t instruction = code[offset];
	switch(instruction){
		case OP_RETURN:{
			std::cout << "OP_RETURN";
			break;
		}
		case OP_CONST:{
			return disasm_const(offset);
			break;
		}
		default:{
			std::cout << "UNKNOWN OPCODE";
		}
	}

	std::cout << '\n';

	return offset + 1;
}

int Chunk::disasm_const(int offset){
	uint8_t constant = code[offset + 1];
	std::cout << "OP_CONST  ";
	// Note: Print `constant` as int, because cout recognize uint8_t as char
	std::cout << std::setfill('0') << std::setw(4) << (int)constant;
	std::cout << " '";
	std::cout << constants.values[constant];
	std::cout << "'\n";
	return offset + 2;
}