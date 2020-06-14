#ifndef PARSER_H
#define PARSER_H

#include <iostream>

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

class Parser {
public:
	Parser(TokenStream & tokens);
	virtual ~Parser() = default;

	ParseTree parse();

private:
	TokenStream & tokens;

	ParseTree tree;

	uint32_t index;

	Token peek();
	Token advance();

	// Token checkers
	bool is_typeof(const TokenType & type);
	bool eof();
	bool is_nl();
	bool is_prefix_op();
	bool is_infix_op();
	bool is_postfix_op();

	bool is_kw(const Keyword & kw);
	bool is_op(const Operator & op);

	// Skippers
	void skip_nl(const bool & optional = false);
	void skip_semis(const bool & optional = false);
	
	void skip_op(const Operator & op,
				 const bool & skip_left_nl = false,
				 const bool & skip_right_nl = false);

	void skip_kw(const Keyword & kw,
				 const bool & skip_left_nl = false,
				 const bool & skip_right_nl = false);

	// Parsers
	NStatement * parse_statement();
	NExpression * parse_expression();
	NExpression * parse_atom();

	NExpression * parse_infix(NExpression * left, int prec);

	NBlock * parse_block();

	NIdentifier * parse_identifier();

	NVarDecl * parse_var_decl();

	NIfExpression * parse_if_expression();

	// Errors
	void unexpected_token();
	void expected_error(const std::string & expected, const std::string & given);
};

#endif