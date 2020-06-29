#ifndef VM_H
#define VM_H

#include <iostream>
#include <vector>
#include <unordered_map>

#include "Exception.h"
#include "backend/Chunk.h"

struct CallFrame {
	Function * function;
	size_t ip;
	std::vector<Value> slots;

	// Chunk manips
	inline uint8_t read_byte(){
		return function->chunk.get_byte(ip++);
	}
	inline Value read_const(){
		return function->chunk.get_const(read_byte());
	}
	inline uint16_t read_short(){
		// Just read two bytes and disjunct them
		ip += 2;
		return ((function->chunk.get_byte(ip - 2) << 8) | (function->chunk.get_byte(ip - 1)));
	}
};

class VM {
public:
	VM(Function * root);
	virtual ~VM() = default;

	void run();

private:
	std::vector<CallFrame> frames;
	CallFrame * frame;

	std::unordered_map<std::string, Value> globals;

	// Stack //
	std::vector<Value> stack;

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

	// Frame chunk wrappers
	inline uint8_t read_byte(){
		return frame->read_byte();
	}
	inline Value read_const(){
		return frame->read_const();
	}
	inline uint16_t read_short(){
		return frame->read_short();
	}

	// Frame chunk manips
	inline std::string read_string(){
		// Read String object from chunk constants and get its inner value
		String * strObj = dynamic_cast<String*>(read_const().obj);
		if(!strObj){
			runtime_error("String expected");
		}
		return strObj->get_val();
	}

	// Errors
	inline void runtime_error(const std::string & msg){
		reset_stack();
		throw RuntimeException(msg);
	}

};

#endif