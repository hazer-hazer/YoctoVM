#ifndef NODE_H
#define NODE_H

#include <memory>

#include "Token.h"
#include "core/Object.h"

struct Node {
	Node() {}
	virtual ~Node() = default;

	Position pos;

	virtual std::string to_string() {
		return "[Node]";
	}

	virtual Object * visit(Scope * scope) {
		return nullptr;
	}
};

struct NStatement : Node {
	NStatement() {}
	virtual ~NStatement() = default;

	virtual std::string to_string() override {
		return "[NStatement]";
	}

	virtual Object * visit(Scope * scope) override {
		return nullptr;
	}
};

struct NExpression : Node {
	NExpression() {}
	virtual ~NExpression() = default;

	virtual std::string to_string() override {
		return "[NExpression]";
	}

	virtual Object * visit(Scope * scope) override {
		return nullptr;
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
	
	virtual Object * visit(Scope * scope) override {
		return expression.visit(scope);
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