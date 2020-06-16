#ifndef NSTRING_H
#define NSTRING_H

#include "Node.h"

struct NString : NExpression {
	Token value;

	NString(const Token & value) : value(value) {}
	virtual ~NString() = default;

	virtual std::string to_string() override {
		return value.String();
	}

	virtual void accept(TreeVisitor & visitor) override {
		visitor.visit(*this);
	}
};

#endif