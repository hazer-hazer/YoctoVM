#ifndef NODE_H
#define NODE_H

#include "Token.h"
#include "core/TreeVisitor.h"

struct Node {
	Node() {}
	virtual ~Node() = default;

	Position pos;

	virtual std::string to_string() {
		return "[Node]";
	}

	virtual void accept(TreeVisitor & visitor) = 0;

	virtual void error(const std::string & msg) = 0;
};

struct NStatement : Node {
	NStatement() {}
	virtual ~NStatement() = default;

	virtual std::string to_string() override {
		return "[NStatement]";
	}

	virtual void accept(TreeVisitor & visitor) = 0;

	virtual void error(const std::string & msg) = 0;
};

struct NExpression : Node {
	NExpression() {}
	virtual ~NExpression() = default;

	virtual std::string to_string() override {
		return "[NExpression]";
	}

	virtual void accept(TreeVisitor & visitor) = 0;

	virtual void error(const std::string & msg) = 0;
};

struct NExpressionStatement : NStatement {
	NExpression & expression;

	NExpressionStatement(NExpression & expression)
		: expression(expression) {}
	virtual ~NExpressionStatement() = default;

	virtual std::string to_string() override {
		return expression.to_string();
	}

	virtual void accept(TreeVisitor & visitor) override {
		expression.accept(visitor);
	}

	virtual void error(const std::string & msg) override {
		expression.error(msg);
	}
};

typedef std::vector<Node*> ParseTree;

typedef std::vector<NStatement*> StatementList;
typedef std::vector<NExpression*> ExpressionList;

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