#include "core/String.h"

#include "core/Bool.h"

String::String(Token & token){
	if(token.type != T_STR){
		token.error("Expected string");
	}
	value = token.String();
}

std::string String::get_value(){
	return value;
}

Bool * String::toBool(){
	// TODO: Think about it...
	return new Bool(value == "true");
}

String * String::toString(){
	return this;
}