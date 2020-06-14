#include "core/TreeVisitor.h"

TreeVisitor::TreeVisitor(){

}

Object * TreeVisitor::visit(NBlock * block){
	std::cout << "visit NBlock" << std::endl;
	return nullptr;
	// Object * value = nullptr;
	// for(NStatement * stmt : block.statements){
	// 	if(stmt){
	// 		value = stmt->eval(scope);
	// 	}
	// }
	// return value;
}

Object * TreeVisitor::visit(NBool * b){
	std::cout << "visit NBool" << std::endl;
	return nullptr;
}

Object * TreeVisitor::visit(NFloat * f){
	std::cout << "visit NFloat" << std::endl;
	return nullptr;
}

Object * TreeVisitor::visit(NIdentifier * id){
	std::cout << "visit NIdentifier" << std::endl;
	return nullptr;
}

Object * TreeVisitor::visit(NIfExpression * if_expr){
	std::cout << "visit NIfExpression" << std::endl;
	return nullptr;

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

Object * TreeVisitor::visit(NInfixOp * infix_op){
	std::cout << "visit NInfixOp" << std::endl;
	return nullptr;

	// Object * lho = left.eval(scope);
	// Object * rho = right.eval(scope);

	// // Note: Assignment has different behavior than other operators
	// // For array access `a[i] = new_value` -> `a.getitem(i)->set(new_value)`,
	// // so `a[i]` must return pointer to Object at index `i`
	// if(op.op() == OP_ASSIGN){
	// 	// NIdentifier * id = dynamic_cast<NIdentifier*>(&left);
	// 	// if(id){
	// 	// 	return scope->assign(*id, rho);
	// 	// }
	// }

	// // switch(op){
	// // 	case OP_ADD:{
	// // 		return lho->call_method("__add", rho);
	// // 		break;
	// // 	}
	// // 	case OP_SUB:{
	// // 		return lho->call_method("__sub", rho);
	// // 		break;
	// // 	}
	// // 	case OP_MUL:{
	// // 		return lho->call_method("__mul", rho);
	// // 		break;
	// // 	}
	// // 	case OP_DIV:{
	// // 		return lho->call_method("__div", rho);
	// // 		break;
	// // 	}
	// // 	case OP_MOD:{
	// // 		return lho->call_method("__mod", rho);
	// // 		break;
	// // 	}
	// // }
}

Object * TreeVisitor::visit(NInt * i){
	std::cout << "visit NInt" << std::endl;
	return nullptr;
}

Object * TreeVisitor::visit(Node * node){
	std::cout << "visit Node" << std::endl;
	return nullptr;
}

Object * TreeVisitor::visit(NPostfixOp * postfix_op){
	std::cout << "visit NPostfixOp" << std::endl;
	return nullptr;
}

Object * TreeVisitor::visit(NPrefixOp * prefix_op){
	std::cout << "visit NPrefixOp" << std::endl;
	return nullptr;
}

Object * TreeVisitor::visit(NVarDecl * var_decl){
	std::cout << "visit NVarDecl" << std::endl;
	return nullptr;
}