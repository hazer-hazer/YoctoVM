#ifndef PRINT_H
#define PRINT_H

#include "tree/Node.h"

// Note: Print might be removed with built-in function in the future

struct Print : Statement {
	Expression & expr;

	Print(Expression & expr) : expr(expr) {}
	virtual ~Print() = default;

	void accept(BaseVisitor & visitor) override {
		visitor.visit(*this);
	}
};

#endif