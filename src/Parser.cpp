#include "Parser.h"

Parser::Parser(const TokenStream & tokens){
	index = 0;
	this->tokens = tokens;
}

Token Parser::peek(){
	return tokens[index];
}

Token Parser::advance(){
	return tokens[++index];
}

/////////////
// Chekers //
/////////////
bool Parser::eof(){
	return is_typeof(T_EOF);
}

bool Parser::is_typeof(const TokenType & type){
	return peek().type == type;
}

bool Parser::is_nl(){
	return is_typeof(T_NL);
}

bool Parser::is_op(const Operator & op){
	return is_typeof(T_OP) && peek().op() == op;
}

bool Parser::is_kw(const Keyword & kw){
	return is_typeof(T_KW) && peek().kw() == kw;
}

bool Parser::is_infix_op(){
	return is_typeof(T_OP) && get_infix_prec(peek().op()) != PREC_NONE;
}

//////////////
// Skippers //
//////////////
void Parser::skip_nl(const bool & optional){
	if(is_nl()){
		do{
			advance();
		}while(is_nl());
	}else if(!optional){
		expected_error("new line");
	}
}

void Parser::skip_semis(){
	if(is_nl() || is_op(OP_SEMICOLON)){
		do{
			advance();
		}while(is_nl() || is_op(OP_SEMICOLON));
	}else{
		expected_error("`;` or new line");
	}
}

void Parser::skip_op(const Operator & op, const bool & skip_l_nl, const bool & skip_r_nl){
	if(skip_l_nl){
		skip_nl(true);
	}
	if(is_op(op)){
		advance();
	}else{
		expected_error(op_to_str(op));
	}
	if(skip_r_nl){
		skip_nl(true);
	}
}

void Parser::skip_kw(const Keyword & kw, const bool & skip_l_nl, const bool & skip_r_nl){
	if(skip_l_nl){
		skip_nl(true);
	}
	if(is_kw(kw)){
		advance();
	}else{
		expected_error(kw_to_str(kw));
	}
	if(skip_r_nl){
		skip_nl(true);
	}
}

/////////////
// Parsers //
/////////////
ParseTree Parser::parse(){
	while(!eof()){
		while(is_nl()){
			advance();
		}
		tree.push_back(parse_statement());

		if(!eof()){
			skip_semis();
		}
	}

	return tree;
}

Statement * Parser::parse_statement(){

	if(is_typeof(T_KW)){
		switch(peek().kw()){
			case KW_VAR:
			case KW_VAL:{
				return parse_var_decl();
				break;
			}
			case KW_FUNC:{
				return parse_func_decl();
				break;
			}
		}
	}

	return new ExprStmt(*parse_expression());
}

Expression * Parser::parse_expression(){
	Expression * left = parse_atom();

	while(!eof()){
		if(is_op(OP_LPAREN)){
			left = parse_func_call(left);
		}else if(is_typeof(T_OP)){
			// Check for infix operator
			if(is_infix_op()){
				return parse_infix(left, 0);
			}
			// TODO: Add else if postfix (and break!)
		}else{
			break;
		}
	}
	
	return left;
}

Expression * Parser::parse_atom(){
	if(is_typeof(T_INT) || is_typeof(T_FLOAT) || is_typeof(T_STR) || is_typeof(T_BOOL)){
		Token current = peek();
		advance();
		return new Literal(current);
	}

	if(is_typeof(T_ID)){
		return parse_id();
	}

	unexpected_error();
	return nullptr;
}

Identifier * Parser::parse_id(){
	if(!is_typeof(T_ID)){
		expected_error("identifier");
	}

	Identifier * id = new Identifier(peek());
	advance();
	return id;
}

Block * Parser::parse_block(){
	skip_op(OP_LBRACE, false, true);

	StatementList stmts;
	
	bool first = true;
	while(!eof()){
		if(is_op(OP_RBRACE)){
			break;
		}
		if(first){
			first = false;
		}else{
			skip_semis();
		}
		if(is_op(OP_RBRACE)){
			break;
		}
		stmts.push_back(parse_statement());
	}
	skip_op(OP_RBRACE, true, false);

	return new Block(stmts);
}

Expression * Parser::parse_infix(Expression * left, int prec){
	if(is_infix_op()){
		Token op_token = peek();
		int right_prec = get_infix_prec(op_token.op());
		if(right_prec > prec){
			advance();
			Expression * maybe_infix = new InfixOp(*left, op_token, *parse_infix(parse_atom(), right_prec));
			return parse_infix(maybe_infix, prec);
		}
	}

	return left;
}

VarDecl * Parser::parse_var_decl(){
	VarDeclType decl = VAR;
	if(is_kw(KW_VAR)){
		decl = VAR;
	}else if(is_kw(KW_VAL)){
		decl = VAL;
	}else{
		expected_error("`var` or `val` keyword");
	}
	advance();

	Identifier * id = parse_id();

	Expression * assign_expr = nullptr;
	if(is_op(OP_ASSIGN)){
		skip_op(OP_ASSIGN, true, true);
		assign_expr = parse_expression();
	}

	return new VarDecl(decl, *id, assign_expr);
}

FuncDecl * Parser::parse_func_decl(){
	skip_kw(KW_FUNC, false, true);

	Identifier * id = parse_id();

	skip_op(OP_LPAREN, true, true);
	
	ParamList params;

	bool first = true;
	while(!eof()){
		if(is_op(OP_RPAREN)){
			break;
		}
		if(first){
			first = false;
		}else{
			skip_op(OP_COMMA, true, true);
		}
		Identifier * param_id = parse_id();
		params.push_back({*param_id});
	}
	skip_op(OP_RPAREN, true, true);

	Block * body = parse_block();

	return new FuncDecl(*id, params, *body);
}

FuncCall * Parser::parse_func_call(Expression * left){
	skip_op(OP_LPAREN, true, true);

	ExpressionList args;

	bool first = true;
	while(!eof()){
		if(is_op(OP_RPAREN)){
			break;
		}
		if(first){
			first = false;
		}else{
			skip_op(OP_COMMA, true, true);
		}
		args.push_back(parse_expression());
	}

	skip_op(OP_RPAREN, true, true);

	return new FuncCall(*left, args);
}

////////////
// Errors //
////////////
void Parser::error(const std::string & msg){
	throw YoctoException(msg);
}

void Parser::unexpected_error(){
	throw UnexpectedTokenException(peek());
}

void Parser::expected_error(const std::string & expected){
	throw ExpectedException(expected, peek());
}