#ifndef BOOL_H
#define BOOL_H

#include "core/DataObject.h"

class Bool : public DataObject {
public:
	Bool(Token & token) {
		if(token.type != T_BOOL){
			token.error("Expected bool");
		}
		value = token.Bool();
	}
	virtual ~Bool() = default;

	virtual std::string to_string() override {
		return std::to_string(value);
	}
	
private:
	bool value;
};

#endif