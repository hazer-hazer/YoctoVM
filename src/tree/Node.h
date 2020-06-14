#ifndef NODE_H
#define NODE_H

#include "Token.h"

struct Node {
	Node() {}
	virtual ~Node() = default;

	Position pos;

	virtual std::string to_string() {
		return "[Node]";
	}
};

struct NStatement : Node {
	NStatement() {}
	virtual ~NStatement() = default;

	virtual std::string to_string() override {
		return "[NStatement]";
	}
};

struct NExpression : Node {
	NExpression() {}
	virtual ~NExpression() = default;

	virtual std::string to_string() override {
		return "[NExpression]";
	}
};

struct NExpressionStatement : NStatement {
	NExpression & expression;

	NExpressionStatement(NExpression & expression)
		: expression(expression) {}
	virtual ~NExpressionStatement() = default;

	virtual std::string to_string() override {
		return expression.to_string();
	}
};

typedef std::vector<Node*> ParseTree;

typedef std::vector<NStatement*> StatementList;
// typedef std::vector<NExpression*> ExpressionList;

inline std::string statements_to_str(const StatementList & stmts){
	std::string str;
	for(const auto & stmt : stmts){
		if(stmt){
			str += stmt->to_string() + "\n";
		}
	}
	return str;
}

#endif