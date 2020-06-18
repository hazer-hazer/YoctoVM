#ifndef FLOAT_H
#define FLOAT_H

#include "core/DataObject.h"

struct Token;

class Bool;
class String;

class Float : public DataObject {
public:
	Float(Token & token);
	Float(const double & value) : value(value) {}
	virtual ~Float() = default;

	double get_value();

	// Extension methods
	Bool * toBool() override;
	String * toString() override;
private:
	double value;
};

#endif