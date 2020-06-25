#ifndef VALUE_H
#define VALUE_H

#include <iostream>
#include <variant>

#define FLOAT_NUM_PRECISION 15

enum class ValueType {
	OBJ
};

struct Value {
	ValueType type;
};

inline std::ostream & operator<<(std::ostream & os, const Value & val){
	os << val.toString().value;
}

#endif