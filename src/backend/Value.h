#ifndef VALUE_H
#define VALUE_H

#include <iostream>
#include "backend/memory.h"

typedef double Value;

struct ValueArray {
	int capacity;
	int count;
	Value * values;

	ValueArray();
	virtual ~ValueArray() = default;

	void init();
	void clear();
	void write(Value val);
};

#endif