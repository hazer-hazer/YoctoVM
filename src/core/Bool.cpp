#include "core/Bool.h"

#include "core/String.h"

Bool::Bool(Token & token){
	if(token.type != T_BOOL){
		token.error("Expected bool");
	}
	value = token.Bool();
}

bool Bool::get_value(){
	return value;
}

// Extension methods

Bool * Bool::toBool(){
	return this;
}

String * Bool::toString(){
	return new String(value ? "true" : "false");
}