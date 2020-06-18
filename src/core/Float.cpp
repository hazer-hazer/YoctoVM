#include "core/Float.h"

#include "core/Bool.h"
#include "core/String.h"

Float::Float(Token & token){
	if(token.type != T_FLOAT){
		token.error("Expected float");
	}
	value = token.Float();
}

double Float::get_value(){
	return value;
}

// Extension methods

Bool * Float::toBool(){
	return new Bool(static_cast<bool>(value));
}

String * Float::toString(){
	return new String(std::to_string(value));
}