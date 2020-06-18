#ifndef STRING_H
#define STRING_H

#include "core/DataObject.h"

struct Token;

class Bool;

class String : public DataObject {
public:
	String(Token & token);
	String(const std::string & value) : value(value) {}
	virtual ~String() = default;

	std::string get_value();

	// Extension methods
	Bool * toBool() override;
	String * toString() override;

private:
	std::string value;
};

#endif