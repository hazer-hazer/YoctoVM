#ifndef NINFIXOP_H
#define NINFIXOP_H

#include "tree/Node.h"
#include <map>

const std::map<Operator, int> infix_precedence {
	{OP_ASSIGN, 3},
	{OP_ADD, 17}, {OP_SUB, 17},
	{OP_MUL, 18}, {OP_DIV, 18}, {OP_MOD, 18}
};

struct NInfixOp : NExpression {
	NExpression & left;
	Token op;
	NExpression & right;

	NInfixOp(NExpression & left, const Token & op, NExpression & right)
		: left(left), op(op), right(right) {}
	virtual ~NInfixOp() = default;

	virtual std::string to_string() override {
		return left.to_string() +" "+ op_to_str(op.op()) +" "+ right.to_string();
	}
};

#endif