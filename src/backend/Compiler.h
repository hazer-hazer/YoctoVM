#ifndef COMPILER_H
#define COMPILER_H

#include "BaseVisitor.h"
#include "nodes.h"

class Compiler : public BaseVisitor {
public:
	Compiler();
	virtual ~Compiler() = default;

	void visit(const ParseTree & tree) override;
	void visit(ExprStmt & expr_stmt) override;
	void visit(Literal & literal) override;
	void visit(Identifier & id) override;
	void visit(VarDecl & var_decl) override;
	void visit(Block & block) override;
	void visit(FuncDecl & func_decl) override;
	void visit(FuncCall & func_call) override;
	void visit(InfixOp & infix_op) override;

private:
	
};

#endif