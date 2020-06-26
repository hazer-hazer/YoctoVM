#include "backend/Compiler.h"

Compiler::Compiler(Chunk & chunk) : chunk(chunk) {
	scope_depth = 0;
}

// Emit
void Compiler::emit(uint8_t byte){
	chunk.write(byte);
}

void Compiler::emit(OpCode op){
	chunk.write(op);
}

void Compiler::emit(OpCode op, uint8_t byte){
	emit(op);
	emit(byte);
}

void Compiler::emit(OpCode op1, OpCode op2){
	emit(op1);
	emit(op2);
}

// JUMP
int Compiler::emit_jump(OpCode op){
	emit(op);
	emit(0xff);
	emit(0xff);
	return chunk.count() - 2;
}

void Compiler::patch_jump(int offset){
	int jump = chunk.count() - offset - 2;

	if(jump > UINT16_MAX){
		throw "Too much code to jump over.";
	}

	chunk.set_byte(offset, (jump >> 8) & 0xff);
	chunk.set_byte(offset + 1, jump & 0xff);
}

// CONST
void Compiler::emit_const(Value val){
	emit(OpCode::Const, make_const(val));
}

uint8_t Compiler::make_const(Value val){
	auto constant = chunk.add_const(val);
	if(constant > UINT8_MAX){
		throw "Too many constants in one chunk";
	}
	return static_cast<uint8_t>(constant);
}

uint8_t Compiler::make_id_const(const std::string & name){
	return make_const({ValueType::DataObj, new String(name)});
}

// LOOP
void Compiler::emit_loop(int loop_start){
	emit(OpCode::SetupLoop);

	int offset = chunk.count() - loop_start + 2;
	if(offset > UINT16_MAX){
		throw "Loop body too large";
	}

	emit((offset >> 8) & 0xff);
	emit(offset & 0xff);
}

// Scope
void Compiler::enter_scope(){
	scope_depth++;
}
void Compiler::exit_scope(){
	// Got to parent scope and pop all locals in exited scope
	scope_depth--;
	while(!locals.empty() && locals.back().depth > scope_depth){
		emit(OpCode::Pop);
		locals.pop_back();
	}
}

// Locals
bool Compiler::is_local(){
	return scope_depth > 0;
}

void Compiler::add_local(const std::string & name){
	// Push new local (but it's not still inited)
	if(locals.size() == UINT8_COUNT){
		throw "Too many variables in function";
	}
	locals.emplace_back(Local(name, -1));
}

void Compiler::mark_inited(){
	// Mark local as initialized if it's not in global scope
	if(scope_depth == 0){
		return;
	}
	locals.back().depth = scope_depth;
}

void Compiler::declare_var(const std::string & name){
	if(scope_depth == 0){
		return;
	}

	for(long i = locals.size() - 1; i >= 0; i--){
		if(locals[i].depth != -1 && locals[i].depth < scope_depth){
			break;
		}
		if(locals[i].name == name){
			throw name + " is already defined";
		}
	}

	add_local(name);
}

int Compiler::resolve_local(const std::string & name){
	for(long i = locals.size() - 1; i >= 0; i--){
		if(locals[i].name == name){
			if(locals[i].depth == -1){
				throw "Cannot read locals variable in its own initializer";
			}
			return static_cast<int>(i);
		}
	}

	return -1;
}

void Compiler::visit(const ParseTree & tree){
	for(Node * n : tree){
		n->accept(*this);
	}

	emit(OpCode::Return);

	chunk.disasm("code");
}

void Compiler::visit(ExprStmt & expr_stmt){
	expr_stmt.expr.accept(*this);
}

void Compiler::visit(Literal & literal){
	switch(literal.token.type){
		case TokenType::Null:{
			emit(OpCode::Null);
			break;
		}
		case TokenType::Int:{
			emit_const({ValueType::DataObj, new Int(literal.token.Int())});
			break;
		}
		case TokenType::Float:{
			emit_const({ValueType::DataObj, new Float(literal.token.Float())});
			break;
		}
		case TokenType::Bool:{
			emit_const({ValueType::DataObj, new Bool(literal.token.Bool())});
			break;
		}
		case TokenType::Str:{
			emit_const({ValueType::DataObj, new String(literal.token.String())});
			break;
		}
	}
	// TODO: Think about exceptions
}

void Compiler::visit(Identifier & id){
	OpCode getOp, setOp;
	auto index = resolve_local(id.get_name());
	if(index == -1){
		// Global
		index = make_id_const(id.get_name());
		emit(OpCode::GetGlobal, static_cast<uint8_t>(index));
	}else{
		emit(OpCode::GetLocal, static_cast<uint8_t>(index));
	}
	// Note: Variable assignments implemented in InfixOp visitor
}

void Compiler::visit(VarDecl & var_decl){
	// Add id to locals
	declare_var(var_decl.id.get_name());
	
	uint8_t global = 0;
	if(!is_local()){
		global = make_id_const(var_decl.id.get_name());
	}

	if(var_decl.assign_expr){
		var_decl.assign_expr->accept(*this);
	}else{
		emit(OpCode::Null);
	}

	if(is_local()){
		// Define local
		mark_inited();
	}else{
		// Define global
		emit(OpCode::DefineGlobal, global);
	}
}

void Compiler::visit(Block & block){
	enter_scope();
	for(Statement * stmt : block.stmts){
		stmt->accept(*this);
	}
	exit_scope();
}

void Compiler::visit(FuncDecl & func_decl){
	std::cout << "visit func_decl\n";
}

void Compiler::visit(FuncCall & func_call){
	std::cout << "visit func_call\n";
}

void Compiler::visit(InfixOp & infix_op){
	std::cout << "visit infix_op\n";
}

void Compiler::visit(IfExpression & if_expr){
	// Note: Now works only with one statement
	
	if_expr.conditions[0].cond.accept(*this);

	int then_jump = emit_jump(OpCode::JumpIfFalse);
	emit(OpCode::Pop);
	if_expr.conditions[0].body.accept(*this);
	int else_jump = emit_jump(OpCode::Jump);

	patch_jump(then_jump);
	emit(OpCode::Pop);

	if(if_expr.Else){
		if_expr.Else->accept(*this);
	}
	patch_jump(else_jump);
}

void Compiler::visit(Print & print){
	print.expr.accept(*this);

	emit(OpCode::Print);
}