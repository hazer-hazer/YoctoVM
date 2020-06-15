#ifndef NVARDECL_H
#define NVARDECL_H

#include "tree/Node.h"

struct NVarDecl : NStatement {
	bool is_val;
	NIdentifier & id;
	NExpression * assign_expr;

	NVarDecl(const bool & is_val, NIdentifier & id, NExpression * assign_expr)
		: is_val(is_val), id(id), assign_expr(assign_expr) {}
	virtual ~NVarDecl() = default;

	std::string get_name(){
		return id.get_name();
	}

	virtual std::string to_string() override {
		return std::string(is_val ? "val" : "var") + " " + id.to_string()
			+ (assign_expr ? assign_expr->to_string() : "");
	}

	virtual void accept(TreeVisitor & visitor) override {
		visitor.visit(*this);
	}
};

#endif