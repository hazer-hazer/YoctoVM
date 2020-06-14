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

private:
	int value;
};

#endif