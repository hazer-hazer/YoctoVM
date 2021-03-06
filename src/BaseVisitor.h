#ifndef BASEVISITOR_H
#define BASEVISITOR_H

#include <vector>

struct Node;
struct ExprStmt;
struct Literal;
struct Identifier;
struct VarDecl;
struct Block;
struct FuncDecl;
struct FuncCall;
struct InfixOp;
struct IfExpression;

struct Print;

using ParseTree = std::vector<Node*>;

class BaseVisitor {
public:
	BaseVisitor() {}
	virtual ~BaseVisitor() = default;
	
	virtual void visit(ExprStmt & expr_stmt) = 0;
	virtual void visit(Literal & literal) = 0;
	virtual void visit(Identifier & id) = 0;
	virtual void visit(VarDecl & var_decl) = 0;
	virtual void visit(Block & block) = 0;
	virtual void visit(FuncDecl & func_decl) = 0;
	virtual void visit(FuncCall & func_call) = 0;
	virtual void visit(InfixOp & infix_op) = 0;
	virtual void visit(IfExpression & if_expr) = 0;
	
	virtual void visit(Print & print) = 0;
};

#endif