#ifndef TREE_VISITOR_H
#define TREE_VISITOR_H

#include "tree/NBlock.h"
#include "tree/NBool.h"
#include "tree/NFloat.h"
#include "tree/NIdentifier.h"
#include "tree/NIfExpression.h"
#include "tree/NInfixOp.h"
#include "tree/NInt.h"
#include "tree/Node.h"
#include "tree/NPostfixOp.h"
#include "tree/NPrefixOp.h"
#include "tree/NVarDecl.h"

#include "core/Object.h"

#include <iostream>

// Note: When enter block or something that is scope
// call method enterScope that creates new scope

class TreeVisitor {
public:
	TreeVisitor();
	virtual ~TreeVisitor() = default;

	Object * visit(NBlock * block);
	Object * visit(NBool * b);
	Object * visit(NFloat * f);
	Object * visit(NIdentifier * id);
	Object * visit(NIfExpression * if_expr);
	Object * visit(NInfixOp * infix_op);
	Object * visit(NInt * i);
	Object * visit(Node * node);
	Object * visit(NPostfixOp * postfix_op);
	Object * visit(NPrefixOp * prefix_op);
	Object * visit(NVarDecl * var_decl);

private:

};

#endif