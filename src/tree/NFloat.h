#ifndef NFLOAT_H
#define NFLOAT_H

#include "tree/Node.h"

struct NFloat : NExpression {
	Token value;

	NFloat(const Token & value) : value(value) {}
	virtual ~NFloat() = default;

	virtual std::string to_string() override {
		return std::to_string(value.Float());
	}
};

#endif