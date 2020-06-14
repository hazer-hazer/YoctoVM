#ifndef NPREFIXOP_H
#define NPREFIXOP_H

#include "tree/Node.h"
#include <set>

// Note: Only Infix operators have precedence
// for prefix and postfix -- postfix has larger precedence than prefix
const std::set<Operator> prefix_operators = {

};

struct NPrefixOp : NExpression {
	Token op;
	NExpression & right;

	NPrefixOp(const Token & op, NExpression & right) : op(op), right(right) {}
	virtual ~NPrefixOp() = default;

	virtual std::string to_string() override {
		return op_to_str(op.op()) +" "+ right.to_string();
	}
};

#endif