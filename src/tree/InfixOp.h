#ifndef INFIXOP_H
#define INFIXOP_H

#include "tree/Node.h"

typedef enum {
	PREC_ZERO,
	PREC_ASSIGN,
	// PREC_OR,
	// PREC_AND,
	// PREC_BIT_OR,
	// PREC_BIT_AND,
	// PREC_EQ,
	// PREC_CMP,
	// PREC_SH,
	// PREC_RANGE,
	PREC_TERM,
	PREC_FACTOR,
	// PREC_EXP,
	// PREC_ACCESS,
	PREC_NONE
} InfixPrec;

inline int get_infix_prec(const Operator & op){
	switch(op){
		case OP_ASSIGN:{
			return PREC_ASSIGN;
			break;
		}
		case OP_ADD:
		case OP_SUB:{
			return PREC_TERM;
			break;
		}
		case OP_MUL:
		case OP_DIV:
		case OP_MOD:{
			return PREC_FACTOR;
			break;
		}
	}
	return PREC_NONE;
}

struct InfixOp : Expression {
	Expression & left;
	Token op;
	Expression & right;

	InfixOp(Expression & left, const Token & op, Expression & right)
		: left(left), op(op), right(right) {}

	void accept(BaseVisitor & visitor) override {
		visitor.visit(*this);
	}
};

#endif