#include "core/TreeVisitor.h"

#include "tree/NBlock.h"
#include "tree/NBool.h"
#include "tree/NFloat.h"
#include "tree/NFuncCall.h"
#include "tree/NFuncDecl.h"
#include "tree/NIdentifier.h"
#include "tree/NIfExpression.h"
#include "tree/NInfixOp.h"
#include "tree/NInt.h"
#include "tree/Node.h"
#include "tree/NPostfixOp.h"
#include "tree/NPrefixOp.h"
#include "tree/NString.h"
#include "tree/NVarDecl.h"

#include "core/Object.h"
#include "core/Bool.h"
#include "core/Float.h"
#include "core/Int.h"
#include "core/Function.h"
#include "core/String.h"

TreeVisitor::TreeVisitor(){
	// Enter global scope
	enter_scope();
}

Scope * TreeVisitor::enter_scope(){
	// Create nested scope with parent of current scope
	return scope = new Scope(scope);
}

void TreeVisitor::exit_scope(){
	// std::cout << "Exit scope (look at these guys before they will be destroyed):\n" + scope->to_string();

	Scope * parent = scope->get_parent();
	delete scope;

	if(!parent){
		throw "Exit global scope";
	}

	scope = parent;
}

void TreeVisitor::visit_tree(const ParseTree & tree){
	for(Node * n : tree){
		if(n){
			n->accept(*this);
		}
	}
	exit_scope();
}

void TreeVisitor::visit(NBlock & block){
	// TODO: Add `must_stop` checker for visiting

	for(NStatement * stmt : block.statements){
		if(stmt){
			stmt->accept(*this);
		}
	}
}

void TreeVisitor::visit(NBool & b){
	// std::cout << "visit NBool\n";
	result = new Bool(b.value);
}

void TreeVisitor::visit(NFloat & f){
	// std::cout << "visit NFloat\n";
	result = new Float(f.value);
}

void TreeVisitor::visit(NFuncCall & func_call){
	// std::cout << "visit NFuncCall\n";

	NIdentifier * id = dynamic_cast<NIdentifier*>(&func_call.left);

	Function * func = nullptr;
	if(id){
		func = scope->lookup_func(id->get_name());
	}else{
		throw "Invalid left-hand side for function call";
	}

	// if(func->params.size() != func_call.args.size()){
	// 	throw "Expected " + std::to_string(func->params.size()) + " arguments";
	// }

	enter_scope();
	
	// for(int i = 0; i < func_call.args.size(); i++){
	// 	// Eval argument and put its value to function scope
	// 	func_call.args[i]->accept(*this);
	// 	scope->define_var(func->params[i].get_name(), get<DataObject>());
	// }

	// visit(func->body);

	func->apply(*this, func_call.args);


	exit_scope();
}

void TreeVisitor::visit(NFuncDecl & func_decl){
	// std::cout << "visit NFuncDecl\n";
	// Note: `result` does not change
		
	UserFunction * user_defined_func = new UserFunction(func_decl);

	scope->define_func(func_decl.get_name(), user_defined_func);
}

void TreeVisitor::visit(NIdentifier & id){
	// std::cout << "visit NIdentifier\n";
	result = scope->lookup_var(id.get_name());
}

void TreeVisitor::visit(NIfExpression & if_expr){
	// std::cout << "visit NIfExpression" << std::endl;

	// for(const auto & c : conditions){
	// 	if(c.condition.eval(scope)){
	// 		return c.body.eval(scope);
	// 	}
	// }
	// if(Else){
	// 	return Else->eval(scope);
	// }

	// return nullptr;
}

void TreeVisitor::visit(NInfixOp & infix_op){
	// std::cout << "visit NInfixOp" << std::endl;

	infix_op.left.accept(*this);
	DataObject * lhs = get<DataObject>();
	if(!lhs){
		throw "Invalid left-hand side";
	}

	infix_op.right.accept(*this);
	DataObject * rhs = get<DataObject>();
	if(!rhs){
		throw "Invalid right-hand side";
	}

	// Note: Assignment has different behavior than other operators
	// For array access `a[i] = new_value` -> `a.getitem(i)->set(new_value)`,
	// so `a[i]` must return pointer to Object at index `i`
	if(infix_op.op.op() == OP_ASSIGN){
		NIdentifier * id = dynamic_cast<NIdentifier*>(&(infix_op.left));
		if(id){
			result = scope->assign_var(id->name.String(), rhs);
		}else{
			throw "Invalid left-hand side in assignment";
		}
	}

	std::vector<NExpression*> right_hand_expr { &infix_op.right };
	switch(infix_op.op.op()){
		case OP_ADD:{
			result = lhs->call_method(*this, "__add", right_hand_expr);
			break;
		}
	}
}

void TreeVisitor::visit(NInt & i){
	// std::cout << "visit NInt\n";
	result = new Int(i.value);
}

void TreeVisitor::visit(NPostfixOp & postfix_op){
	std::cout << "visit NPostfixOp" << std::endl;
}

void TreeVisitor::visit(NPrefixOp & prefix_op){
	std::cout << "visit NPrefixOp" << std::endl;
}

void TreeVisitor::visit(NString & string){
	result = new String(string.value);
}

void TreeVisitor::visit(NVarDecl & var_decl){
	// std::cout << "visit NVarDecl" << std::endl;
	// TODO: Add is_val status
	DataObject * value = nullptr;
	if(var_decl.assign_expr){
		var_decl.assign_expr->accept(*this);
		value = dynamic_cast<DataObject*>(result);
		if(!value){
			throw "Invalid right-hand side in assignment";
		}
	}


	scope->define_var(var_decl.id.name.String(), value);
}

Scope * TreeVisitor::get_scope(){
	return scope;
}