#ifndef INT_H
#define INT_H

#include "core/DataObject.h"

struct Token;

class Bool;
class String;

class Int : public DataObject {
public:
	Int(Token & token);
	Int(const int & value) : value(value) {}
	virtual ~Int() = default;

	int get_value();

	// Extension methods
	Bool * toBool() override;
	String * toString() override;

private:
	int value;
};

#endif