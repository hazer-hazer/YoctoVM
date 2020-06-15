#ifndef BOOL_H
#define BOOL_H

#include "core/DataObject.h"

class Bool : public DataObject {
public:
	Bool(Token & token) : DataObject() {
		if(token.type != T_BOOL){
			throw "Expected bool token";
		}
		value = token.Bool();
	}
	Bool(const bool & value) : DataObject(), value(value) {}
	virtual ~Bool() = default;

	virtual std::string to_string() override {
		return std::to_string(value);
	}
	
private:
	bool value;
};

#endif