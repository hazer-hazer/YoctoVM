#ifndef IDENTIFIER_H
#define IDENTIFIER_H

#include "tree/Node.h"

struct Identifier : Expression {
	Token token;

	Identifier(const Token & token) : token(token) {}

	void accept(BaseVisitor & visitor) override {
		visitor.visit(*this);
	}
};

#endif