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
	return is_typeof(TokenType::T_EOF);
}

bool Parser::is_typeof(const TokenType & type){
	return peek().type == type;
}

bool Parser::is_nl(){
	return is_typeof(TokenType::T_NL);
}

bool Parser::is_op(const Operator & op){
	return is_typeof(TokenType::T_OP) && peek().op() == op;
}

bool Parser::is_kw(const Keyword & kw){
	return is_typeof(TokenType::T_KW) && peek().kw() == kw;
}

bool Parser::is_infix_op(){
	return is_typeof(TokenType::T_OP) && get_infix_prec(peek().op()) != static_cast<int>(InfixPrec::NONE);
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
	if(is_nl() || is_op(Operator::SEMICOLON)){
		do{
			advance();
		}while(is_nl() || is_op(Operator::SEMICOLON));
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
	std::cout << "parse_statement: " << peek().to_string() << std::endl;

	if(is_typeof(TokenType::T_KW)){
		switch(peek().kw()){
			case Keyword::KW_VAR:
			case Keyword::KW_VAL:{
				return parse_var_decl();
				break;
			}
			case Keyword::KW_FUNC:{
				return parse_func_decl();
				break;
			}
			case Keyword::KW_PRINT:{
				skip_kw(Keyword::KW_PRINT, false, false);
				Expression * expr = parse_expression();
				return new Print(*expr);
			}
		}
	}

	return new ExprStmt(*parse_expression());
}

Expression * Parser::parse_expression(){
	std::cout << "parse_expression: " << peek().to_string() << std::endl;
	Expression * left = parse_atom();

	while(!eof()){
		if(is_op(Operator::LPAREN)){
			left = parse_func_call(left);
		}else if(is_infix_op()){
			return parse_infix(left, 0);
			// TODO: Add else if postfix (and break!)
		}else{
			break;
		}
	}
	
	return left;
}

Expression * Parser::parse_atom(){
	std::cout << "parse_atom: " << peek().to_string() << std::endl;
	if(is_typeof(TokenType::T_INT) || is_typeof(TokenType::T_FLOAT) || is_typeof(TokenType::T_STR) || is_typeof(TokenType::T_BOOL)){
		Token current = peek();
		advance();
		return new Literal(current);
	}

	if(is_typeof(TokenType::T_ID)){
		return parse_id();
	}

	if(is_kw(Keyword::KW_IF)){
		return parse_if_expr();
	}

	unexpected_error();
	return nullptr;
}

Identifier * Parser::parse_id(){
	if(!is_typeof(TokenType::T_ID)){
		expected_error("identifier");
	}

	Identifier * id = new Identifier(peek());
	advance();
	return id;
}

Block * Parser::parse_block(){
	std::cout << "parse block: " << peek().to_string() << std::endl;
	skip_op(Operator::LBRACE, false, true);

	StatementList stmts;
	
	bool first = true;
	while(!eof()){
		if(is_op(Operator::RBRACE)){
			break;
		}
		if(first){
			first = false;
		}else{
			skip_semis();
		}
		if(is_op(Operator::RBRACE)){
			break;
		}
		stmts.push_back(parse_statement());
	}
	skip_op(Operator::RBRACE, true, false);

	return new Block(stmts);
}

Expression * Parser::parse_infix(Expression * left, int prec){
	std::cout << "parse infix: " << peek().to_string() << std::endl;
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
	std::cout << "parse VarDecl: " << peek().to_string() << std::endl;
	VarDeclType decl = VarDeclType::VAR;
	if(is_kw(Keyword::KW_VAR)){
		decl = VarDeclType::VAR;
	}else if(is_kw(Keyword::KW_VAL)){
		decl = VarDeclType::VAL;
	}else{
		expected_error("`var` or `val` keyword");
	}
	advance();

	Identifier * id = parse_id();

	Expression * assign_expr = nullptr;
	if(is_op(Operator::ASSIGN)){
		skip_op(Operator::ASSIGN, true, true);
		assign_expr = parse_expression();
	}

	return new VarDecl(decl, *id, assign_expr);
}

FuncDecl * Parser::parse_func_decl(){
	std::cout << "parse FuncDecl: " << peek().to_string() << std::endl;
	skip_kw(Keyword::KW_FUNC, false, true);

	Identifier * id = parse_id();

	skip_op(Operator::LPAREN, true, true);
	
	ParamList params;

	bool first = true;
	while(!eof()){
		if(is_op(Operator::RPAREN)){
			break;
		}
		if(first){
			first = false;
		}else{
			skip_op(Operator::COMMA, true, true);
		}
		Identifier * param_id = parse_id();
		params.push_back({*param_id});
	}
	skip_op(Operator::RPAREN, true, true);

	Block * body = parse_block();

	return new FuncDecl(*id, params, *body);
}

FuncCall * Parser::parse_func_call(Expression * left){
	std::cout << "parse FuncCall: " << peek().to_string() << std::endl;
	skip_op(Operator::LPAREN, true, true);

	ExpressionList args;

	bool first = true;
	while(!eof()){
		if(is_op(Operator::RPAREN)){
			break;
		}
		if(first){
			first = false;
		}else{
			skip_op(Operator::COMMA, true, true);
		}
		args.push_back(parse_expression());
	}

	skip_op(Operator::RPAREN, true, true);

	return new FuncCall(*left, args);
}

IfExpression * Parser::parse_if_expr(){
	std::cout << "parse if: " << peek().to_string() << std::endl;
	ConditionList conditions;

	// Note: do-while because there always must first condition-struct (`if`)
	
	// Parse if
	skip_kw(Keyword::KW_IF, true, true);
	skip_op(Operator::LPAREN, true, true);
	Expression * IfCond = parse_expression();
	skip_op(Operator::RPAREN, true, true);
	Block * IfBody = parse_block();

	conditions.push_back(ConditionStructure(*IfCond, *IfBody));

	while(!eof()){
		if(is_kw(Keyword::KW_ELIF)){
			std::cout << "elif found\n";
			skip_kw(Keyword::KW_ELIF, true, true);
		}else{
			std::cout << "end condition\n";
			break;
		}

		skip_op(Operator::LPAREN, true, true);
		Expression * cond = parse_expression();
		skip_op(Operator::RPAREN, true, true);
		Block * body = parse_block();

		conditions.push_back(ConditionStructure(*cond, *body));
	}

	Block * Else = nullptr;
	if(is_kw(Keyword::KW_ELSE)){
		skip_kw(Keyword::KW_ELSE, true, true);
		Else = parse_block();
	}

	return new IfExpression(conditions, Else);
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