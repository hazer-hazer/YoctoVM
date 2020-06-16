#ifndef STRING_H
#define STRING_H

#include "DataObject.h"

class String : public DataObject {
public:
	String(Token & token){
		if(token.type != T_STR){
			token.error("Expected string");
		}
		value = token.String();
	}
	virtual ~String() = default;

	virtual std::string to_string() override {
		return value;
	}

private:
	std::string value;
};

#endif