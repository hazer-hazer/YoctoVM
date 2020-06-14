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

	virtual Object * visit(Scope * scope) override {
		Object * lho = left.eval(scope);
		Object * rho = right.eval(scope);
		// Note: Assignment has different behavior than other operators
		// For array access `a[i] = new_value` -> `a.getitem(i)->set(new_value)`,
		// so `a[i]` must return pointer to Object at index `i`
		if(op == OP_ASSIGN){
			// NIdentifier * id = dynamic_cast<NIdentifier*>(&left);
			// if(id){
			// 	return scope->assign(*id, rho);
			// }
		}

		switch(op){
			case OP_ADD:{
				return lho->call_method("__add", rho);
				break;
			}
			case OP_SUB:{
				return lho->call_method("__sub", rho);
				break;
			}
			case OP_MUL:{
				return lho->call_method("__mul", rho);
				break;
			}
			case OP_DIV:{
				return lho->call_method("__div", rho);
				break;
			}
			case OP_MOD:{
				return lho->call_method("__mod", rho);
				break;
			}
		}
	}
};

#endif