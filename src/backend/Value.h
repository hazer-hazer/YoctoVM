#ifndef VALUE_H
#define VALUE_H

#include <iostream>
#include <variant>
#include <iomanip>

#define FLOAT_NUM_PRECISION 15

struct Object {

};

struct Value {
	ValueType	
};

using Value = Object;

struct ValuePrinter {
	void operator()(const double d) const {
		std::cout << std::setprecision(FLOAT_NUM_PRECISION) << d;
	}
	void operator()(const bool b) const {
		std::cout << b;
	}
	void operator()(const int i) const {
		std::cout << i;
	}
	void operator()(const std::monostate n) const {
		std::cout << "null";
	}
	void operator()(const std::string s) const {
		std::cout << s;
	}
};

inline std::ostream & operator<<(std::ostream & os, const Value & val){
	os << val.toString().value;
}

#endif