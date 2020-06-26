#include "Object/String.h"
#include "Object/Bool.h"

String::String(const std::string & s) : value(s) {}

Bool * String::toBool(){
	// TODO: Add case-insensitivity
	return new Bool(value == "true" ? true : false);
}

String * String::toString(){
	return new String(value);
}