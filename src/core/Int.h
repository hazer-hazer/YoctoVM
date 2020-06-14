#ifndef INT_H
#define INT_H

#include "core/DataObject.h"

class Int {
public:
	Int(const Token & token) : DataObject() {
		if(token.type != T_INT){
			throw "Expected int token";
		}else{
			value = token.Int();
		}
	}
	Int(const int & value) : DataObject(), value(value) {}
	virtual ~Int() = default;

	int getValue(){
		return value;
	}

	operator bool(){
		return value != 0;
	}

private:
	int value;
};

#endif