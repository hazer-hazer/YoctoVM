#ifndef NINT_H
#define NINT_H

#include "tree/Node.h"

struct NInt : NExpression {
	Token value;

	NInt(const Token & value) : value(value) {}
	virtual ~NInt() = default;

	virtual std::string to_string() override {
		return std::to_string(value.Int());
	}

	virtual Object * visit(Scope * scope) override {
		return new Int(value);
	}
};

#endif