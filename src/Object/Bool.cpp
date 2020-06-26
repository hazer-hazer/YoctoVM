#include "Object/Bool.h"
#include "Object/String.h"

Bool::Bool(const bool & b) : value(b) {}

Bool * Bool::toBool(){
	return new Bool(value);
}

String * Bool::toString(){
	return new String(value ? "true" : "false");
}