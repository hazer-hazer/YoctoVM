#ifndef COGEGEN_H
#define CODEGEN_H

#include "BaseVisitor.h"

class Compiler : public BaseVisitor {
public:
	Compiler();
	virtual ~Compiler() = default;

	Function * compile(const ParseTree & tree);

	void visit(ExprStmt & expr_stmt) override;
	void visit(Literal & literal) override;
	void visit(Identifier & id) override;
	void visit(VarDecl & var_decl) override;
	void visit(Block & block) override;
	void visit(FuncDecl & func_decl) override;
	void visit(FuncCall & func_call) override;
	void visit(InfixOp & infix_op) override;
	void visit(IfExpression & if_expr) override;

	void visit(Print & print) override;
};

#endif