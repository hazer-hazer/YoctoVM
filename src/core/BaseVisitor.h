#ifndef BASEVISITOR_H
#define BASEVISITOR_H

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
struct NVarDecl;

typedef std::vector<Node*> ParseTree;

class BaseVisitor {

	virtual void visit_tree(const ParseTree & tree) = 0;

	virtual void visit(NBlock & block) = 0;
	virtual void visit(NBool & b) = 0;
	virtual void visit(NFloat & f) = 0;
	virtual void visit(NFuncCall & func_call) = 0;
	virtual void visit(NFuncDecl & func_decl) = 0;
	virtual void visit(NIdentifier & id) = 0;
	virtual void visit(NIfExpression & if_expr) = 0;
	virtual void visit(NInfixOp & infix_op) = 0;
	virtual void visit(NInt & i) = 0;
	virtual void visit(NPostfixOp & postfix_op) = 0;
	virtual void visit(NPrefixOp & prefix_op) = 0;
	virtual void visit(NVarDecl & var_decl) = 0;

	virtual Scope * get_scope() = 0;

};

#endif