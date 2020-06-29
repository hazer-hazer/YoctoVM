#include "backend/Chunk.h"

Chunk::Chunk(){}

void Chunk::write(uint8_t byte){
	code.push_back(byte);
}

void Chunk::write(OpCode opcode){
	write(static_cast<uint8_t>(opcode));
}

size_t Chunk::add_const(Value val){
	constants.push_back(val);
	return constants.size() - 1;
}

///////////
// Debug //
///////////
void Chunk::disasm(const std::string & chunk_name){
	std::cout << "== " << chunk_name << " ==" << std::endl;

	for(int offset = 0; offset < code.size();){
		offset = disasm_instruction(offset);
	}
}

static int simple_instruction(const std::string & name, int offset){
	std::cout << name << std::endl;
	return offset + 1;
}

static int const_instruction(const std::string & name, const Chunk & chunk, int offset){
	auto constant = chunk.get_byte(offset + 1);
	printf("%-16s %4d '", name.c_str(), constant);
	std::cout << chunk.get_const(constant);
	printf("'\n");
	return offset + 2;
}

static int byte_instruction(const std::string & name, const Chunk & chunk, int offset){
	auto slot = chunk.get_byte(offset + 1);
	printf("%-16s %4d\n", name.c_str(), slot);
	return offset + 2;
}

static int jump_instruction(const std::string & name, int sign, const Chunk & chunk, int offset){
	uint16_t jump = static_cast<uint16_t>(chunk.get_byte(offset + 1) << 8);
	jump |= static_cast<uint16_t>(chunk.get_byte(offset + 2));
	printf("%-16s %4d -> %d\n", name.c_str(), offset, offset + 3 + sign * jump);
	return offset + 3;
}

int Chunk::disasm_instruction(int offset){
	printf("%04d ", offset);

	// if(offset > 0 && lines[offset] == lines[offset - 1]){
	// 	std::cout << "   |";
	// }else{
	// 	std::cout << std::setfill('0') << std::setw(4) << lines[offset];
	// }

	// std::cout << ' ';

	auto instruction = OpCode(code[offset]);
	switch(instruction){
		case OpCode::Const:{
			return const_instruction("CONST", *this, offset);
		}
		case OpCode::Null:{
			return simple_instruction("NULL", offset);
		}
		case OpCode::Pop:{
			return simple_instruction("POP", offset);
		}
		case OpCode::DefineGlobal:{
			return const_instruction("DEFINE_GLOBAL", *this, offset);
		}
		case OpCode::GetGlobal:{
			return const_instruction("GET_GLOBAL", *this, offset);
		}
		case OpCode::SetGlobal:{
			return const_instruction("SET_GLOBAL", *this, offset);
		}
		case OpCode::GetLocal:{
			return byte_instruction("GET_LOCAL", *this, offset);
		}
		case OpCode::SetLocal:{
			return byte_instruction("SET_LOCAL", *this, offset);
		}
		case OpCode::Jump:{
			return jump_instruction("JUMP", 1, *this, offset);
		}
		case OpCode::JumpIfFalse:{
			return jump_instruction("JUMP_IF_FALSE", 1, *this, offset);
		}
		case OpCode::Print:{
			return simple_instruction("PRINT", offset);
		}
		case OpCode::StopCode:{
			return simple_instruction("STOP_CODE", offset);
			break;
		}
	}

	std::cout << "Unknown opcode: " << code[offset] << std::endl;

	return offset + 1;
}