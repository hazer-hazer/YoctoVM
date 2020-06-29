#ifndef COMPILER_H
#define COMPILER_H

#include "BaseVisitor.h"
#include "tree/nodes.h"
#include "backend/Chunk.h"
#include "backend/Value.h"

#define UINT8_COUNT (UINT8_MAX + 1)

struct Local {
	std::string name;
	int depth;
	Local(const std::string & name, int depth) : name(name), depth(depth) {}
};

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

private:
	std::vector<Local> locals;

	Function * function;
	FunctionType function_type;

	inline Chunk * current_chunk() const {
		return &function->chunk;
	}

	Function * end_compiler();

	// Emit
	void emit(uint8_t byte);
	void emit(OpCode op);
	void emit(OpCode op, uint8_t byte);
	void emit(OpCode op1, OpCode op2);

	// JUMP
	int emit_jump(OpCode op);
	void patch_jump(int offset);

	// CONST
	void emit_const(Value val);
	uint8_t make_const(Value val);
	uint8_t make_id_const(const std::string & name);

	// LOOP
	void emit_loop(int loop_start);

	// Scope
	int scope_depth;
	void enter_scope();
	void exit_scope();

	bool is_local();
	void add_local(const std::string & name);
	void mark_inited();
	int resolve_local(const std::string & name);

	void declare_var(const std::string & name);

};

#endif