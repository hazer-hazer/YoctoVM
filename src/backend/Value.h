#ifndef VALUE_H
#define VALUE_H

#include <iostream>
#include <vector>

typedef double Value;

struct ValueArray {
	std::vector<Value> values;

	ValueArray();
	virtual ~ValueArray() = default;

	void write(Value val);
};

#endif