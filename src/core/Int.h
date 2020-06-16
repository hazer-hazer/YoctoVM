#ifndef INT_H
#define INT_H

#include "core/DataObject.h"
#include "core/Function.h"

class Int : public DataObject {
public:
	Int(Token & token){
		if(token.type != T_INT){
			token.error("Expected int");
		}
		value = token.Int();
	
		std::vector<std::string> __add_params { "another" };		
		extend_method("__add", new BuiltinFunction(__add_params, [&](BuiltinFuncArgs args){
			Int * another = dynamic_cast<Int*>(args["another"]);
			if(!another){
				throw "Expected Int";
			}
			return new Int(another->get_value() + value);
		}));
	}
	Int(const int & value) : value(value) {}
	virtual ~Int() = default;

	int get_value(){
		return value;
	}

	virtual std::string to_string() override {
		return std::to_string(value);
	}

private:
	int value;
};

#endif