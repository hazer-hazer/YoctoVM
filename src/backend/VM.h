#ifndef VM_H
#define VM_H

#include <iostream>
#include <vector>
#include <unordered_map>

#include "Exception.h"
#include "backend/Chunk.h"

enum InterpretResult {
	OK,
	COMPILE_ERROR,
	RUNTIME_ERROR
};

class VM {
public:
	VM(Chunk & chunk);
	virtual ~VM() = default;

	InterpretResult run();

private:
	// Chunk
	Chunk & chunk;
	unsigned ip;

	std::vector<Value> stack;
	std::unordered_map<std::string, Value> globals;

	inline void reset_stack(){
		stack.clear();
	}

	inline void push(Value val){
		stack.push_back(val);
	}
	inline Value pop(){
		auto v = stack.back();
		stack.pop_back();
		return v;
	}
	inline Value peek(int distance){
		return stack[stack.size() - 1 - distance];
	}

	// Chunk manips
	inline uint8_t read_byte(){
		return chunk.get_byte(ip++);
	}
	inline Value read_const(){
		return chunk.get_const(read_byte());
	}
	inline uint16_t read_short(){
		// Just read two bytes and disjunct them
		ip += 2;
		return ((chunk.get_byte(ip - 2) << 8) | (chunk.get_byte(ip - 1)));
	}
	inline std::string read_string(){
		// Read String object from chunk constants and get its inner value
		String * strObj = dynamic_cast<String*>(read_const().obj);
		if(!strObj){
			runtime_error("String expected");
		}
		return strObj->get_val();
	}

	// Stack manips
	inline bool peek_bool(int distance){
		DataObject * dataObj = dynamic_cast<DataObject*>(peek(distance).obj);
		if(!dataObj){
			// Note: We know that all DataObject's have toBool function, so we throw exception
			runtime_error("Unable to convert expression to boolean");
		}
		// Note: Automatically converts any DataObject to Bool
		// TODO: Think about bad cases !!!
		return dataObj->toBool()->get_val();
	}

	// Errors
	inline void runtime_error(const std::string & msg){
		reset_stack();
		throw YoctoException(msg);
	}

};

#endif