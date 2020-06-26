#include "Object/Int.h"
#include "Object/Bool.h"
#include "Object/String.h"

Int::Int(const int & i) : value(i) {}

Bool * Int::toBool(){
	return new Bool(static_cast<bool>(value));
}

String * Int::toString(){
	return new String(std::to_string(value));
}