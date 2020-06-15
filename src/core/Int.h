#ifndef INT_H
#define INT_H

#include "core/DataObject.h"

class Int : public DataObject {
public:
	Int(Token & token) : DataObject() {
		if(token.type != T_INT){
			throw "Expected int token";
		}else{
			value = token.Int();
		}
	}
	Int(const int & value) : DataObject(), value(value) {}
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