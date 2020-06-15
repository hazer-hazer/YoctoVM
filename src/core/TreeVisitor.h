#ifndef TREE_VISITOR_H
#define TREE_VISITOR_H

#include <iostream>
#include <vector>

#include "core/Scope.h"
#include "core/BaseVisitor.h"

class Object;
class DataObject;

// Note: When enter block or something that is scope
// call method enter_scope that creates new scope

class TreeVisitor : public BaseVisitor {
public:
	TreeVisitor();
	virtual ~TreeVisitor() = default;

	void visit_tree(const ParseTree & tree) override;

	void visit(NBlock & block) override;
	void visit(NBool & b) override;
	void visit(NFloat & f) override;
	void visit(NFuncCall & func_call) override;
	void visit(NFuncDecl & func_decl) override;
	void visit(NIdentifier & id) override;
	void visit(NIfExpression & if_expr) override;
	void visit(NInfixOp & infix_op) override;
	void visit(NInt & i) override;
	void visit(NPostfixOp & postfix_op) override;
	void visit(NPrefixOp & prefix_op) override;
	void visit(NVarDecl & var_decl) override;

	// Get last evaluated result
	template <typename T>
	T * get(){
		return dynamic_cast<T*>(result);
	}

	Scope * get_scope() override;

private:
	Scope * scope;
	Scope * enter_scope();
	void exit_scope();

	DataObject * result;
};

#endif