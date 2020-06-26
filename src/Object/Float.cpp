#include "Object/Float.h"
#include "Object/Bool.h"
#include "Object/String.h"

Float::Float(const double & d) : value(d) {}

Bool * Float::toBool(){
	return new Bool(static_cast<bool>(value));
}

String * Float::toString(){
	return new String(std::to_string(value));
}