#ifndef BOOL_H
#define BOOL_H

#include "core/DataObject.h"

struct Token;

class String;

class Bool : public DataObject {
public:
	Bool(Token & token);
	Bool(const bool & value) : value(value) {}
	virtual ~Bool() = default;

	bool get_value();
	
	// Extension methods
	Bool * toBool() override;
	String * toString() override;

private:
	bool value;
};

#endif