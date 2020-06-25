#ifndef INFIXOP_H
#define INFIXOP_H

#include "tree/Node.h"

enum class InfixPrec {
	ZERO,
	ASSIGN,
	// OR,
	// AND,
	// BIT_OR,
	// BIT_AND,
	// EQ,
	// CMP,
	// SH,
	// RANGE,
	TERM,
	FACTOR,
	// EXP,
	// ACCESS,
	NONE
};

inline int get_infix_prec(const Operator & op){
	InfixPrec prec;
	switch(op){
		case Operator::ASSIGN:{
			prec = InfixPrec::ASSIGN;
			break;
		}
		case Operator::ADD:
		case Operator::SUB:{
			prec = InfixPrec::TERM;
			break;
		}
		case Operator::MUL:
		case Operator::DIV:
		case Operator::MOD:{
			prec = InfixPrec::FACTOR;
			break;
		}
		default:{
			prec = InfixPrec::NONE;
		}
	}

	return static_cast<int>(prec);
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