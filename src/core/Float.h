#ifndef FLOAT_H
#define FLOAT_H

#include "core/DataObject.h"

class Float : public DataObject {
public:
	Float(Token & token) : DataObject() {
		if(token.type != T_FLOAT){
			throw "Expected float token";
			// token.error("unexpected token");
		}else{
			value = token.Float();
		}
	}
	Float(const double & value) : DataObject(), value(value) {}
	virtual ~Float() = default;

private:
	double value;
};

#endif