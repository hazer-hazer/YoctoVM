#ifndef STRING_H
#define STRING_H

#include <string>

#include "Object/DataObject.h"

class String : public DataObject {
public:
	String(const std::string & s);
	virtual ~String() = default;

	Bool * toBool() override;
	String * toString() override;

	std::string get_val() const {
		return value;
	}

private:
	std::string value;
};

#endif