#ifndef FLOAT_H
#define FLOAT_H

#include "core/DataObject.h"

class Float : public DataObject {
public:
	Float(Token & token){
		if(token.type != T_FLOAT){
			token.error("Expected float");
		}
		value = token.Float();
	}
	virtual ~Float() = default;

	virtual std::string to_string() override {
		return std::to_string(value);
	}

private:
	double value;
};

#endif