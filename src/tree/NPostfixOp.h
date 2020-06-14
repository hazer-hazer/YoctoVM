#ifndef NPOSTFIXOP_H
#define NPOSTFIXOP_H

#include "tree/Node.h"
#include <set>

// Note: Only Infix operators have precedence
// for prefix and postfix -- postfix has larger precedence than prefix
const std::set<Operator> postfix_operators = {

};

struct NPostfixOp : NExpression {
	NExpression & left;
	Token op;

	NPostfixOp(NExpression & left, const Token & op) : left(left), op(op) {}
	virtual ~NPostfixOp() = default;

	virtual std::string to_string() override {
		return left.to_string() + " " + op_to_str(op.op()); 
	}
};

#endif