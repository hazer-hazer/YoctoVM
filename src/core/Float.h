#ifndef FLOAT_H
#define FLOAT_H

#include "core/DataObject.h"
#include "core/Bool.h"

class Float : DataObject {
public:
	Float(const Token & token) : DataObject() {
		if(token.type != T_FLOAT){
			throw "Expected float token";
			// token.error("unexpected token");
		}else{
			value = token.Float();
		}
	}
	Float(const double & value) : DataObject(), value(value) {}
	virtual ~Float() = default;
	
	double getValue(){
		return value;
	}

	operator bool(){
		return value != 0.0;
	}

private:
	double value;
};

#endif