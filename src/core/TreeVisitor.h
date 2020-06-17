#ifndef TREE_VISITOR_H
#define TREE_VISITOR_H

#include <iostream>
#include <vector>

#include "core/Scope.h"

struct NBlock;
struct NBool;
struct NFloat;
struct NFuncCall;
struct NFuncDecl;
struct NIdentifier;
struct NIfExpression;
struct NInfixOp;
struct NInt;
struct Node;
struct NPostfixOp;
struct NPrefixOp;
struct NString;
struct NVarDecl;

typedef std::vector<Node*> ParseTree;

class Object;
class DataObject;

// Note: When enter block or something that is scope
// call method enter_scope that creates new scope

class TreeVisitor {
public:
	TreeVisitor();
	virtual ~TreeVisitor() = default;

	virtual void visit_tree(const ParseTree & tree);

	virtual void visit(NBlock & block);
	virtual void visit(NBool & b);
	virtual void visit(NFloat & f);
	virtual void visit(NFuncCall & func_call);
	virtual void visit(NFuncDecl & func_decl);
	virtual void visit(NIdentifier & id);
	virtual void visit(NIfExpression & if_expr);
	virtual void visit(NInfixOp & infix_op);
	virtual void visit(NInt & i);
	virtual void visit(NPostfixOp & postfix_op);
	virtual void visit(NPrefixOp & prefix_op);
	virtual void visit(NString & string);
	virtual void visit(NVarDecl & var_decl);

	// Get last evaluated result
	template <typename T>
	T * get(){
		return dynamic_cast<T*>(result);
	}

	Scope * get_scope();

private:
	Scope * scope;
	Scope * enter_scope();
	void exit_scope();

	DataObject * result;
};

#endif