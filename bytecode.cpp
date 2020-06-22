#include "src/backend/Chunk.h"
#include "src/backend/VM.h"

int main(int argc, const char * argv[]){
	Chunk chunk;

	int constant = chunk.add_const(1.2);
	chunk.write(OP_CONST, 123);
	chunk.write(constant, 123);
	chunk.write(OP_NEGATE, 123);

	chunk.write(OP_RETURN, 123);

	VM vm;
	vm.interpret(&chunk);

	return 0;
}