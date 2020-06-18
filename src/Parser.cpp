#include "Parser.h"

Parser::Parser(TokenStream & tokens) : tokens(tokens) {
	index = 0;
}

Token Parser::peek(){
	return tokens[index];
}
Token Parser::advance(){
	return tokens[++index];
}

// Token checkers
bool Parser::is_typeof(const TokenType & type){
	return peek().type == type;
}
bool Parser::eof(){
	return peek().type == T_EOF;
}
bool Parser::is_nl(){
	return peek().type == T_NL;
}
bool Parser::is_prefix_op(){
	return is_typeof(T_OP)
		&& prefix_operators.find(peek().op()) != prefix_operators.end();
}
bool Parser::is_infix_op(){
	return is_typeof(T_OP)
		&& infix_precedence.find(peek().op()) != infix_precedence.end();
}
bool Parser::is_postfix_op(){
	return is_typeof(T_OP)
		&& postfix_operators.find(peek().op()) != postfix_operators.end();
}
bool Parser::is_kw(const Keyword & kw){
	return is_typeof(T_KW) && peek().kw() == kw;
}
bool Parser::is_op(const Operator & op){
	return is_typeof(T_OP) && peek().op() == op;
}

// Skipers
void Parser::skip_nl(const bool & optional){
	if(is_nl()){
		do{
			std::cout << "skip_nl" << std::endl;
			advance();
		}while(is_nl());
	}else if(!optional){
		expected_error("[newline]", "");
	}
}
void Parser::skip_semis(const bool & optional){
	if(is_nl() || is_op(OP_SEMICOLON)){
		do{
			advance();
		}while(is_nl() || is_op(OP_SEMICOLON));
	}else if(!optional){
		expected_error("`;` or [newline]", "");
	}
}
void Parser::skip_op(const Operator & op, const bool & skip_left_nl, const bool & skip_right_nl){
	if(skip_left_nl){
		skip_nl(true);
	}
	if(is_op(op)){
		advance();
	}else{
		expected_error("operator `"+ op_to_str(op) +"`", "");
	}
	if(skip_right_nl){
		skip_nl(true);
	}
}
void Parser::skip_kw(const Keyword & kw, const bool & skip_left_nl, const bool & skip_right_nl){
	if(skip_left_nl){
		skip_nl(true);
	}
	if(is_kw(kw)){
		advance();
	}else{
		expected_error("keyword `"+ kw_to_str(kw) +"`", "");
	}
	if(skip_right_nl){
		skip_nl(true);
	}
}

ParseTree Parser::parse(){
	while(!eof()){
		NStatement * stmt = parse_statement();
		if(stmt){
			tree.push_back(stmt);
		}
		if(!eof()){
			// TODO: Check for parsing with nl
			skip_semis();
		}
	}

	return tree;
}

/////////////
// Parsers //
/////////////

NStatement * Parser::parse_statement(){
	std::cout << "parse_statement: " << peek().to_string() << std::endl;

	if(is_typeof(T_KW)){
		switch(peek().kw()){
			case KW_VAL:
			case KW_VAR:{
				return parse_var_decl();
				break;
			}
			case KW_FUNC:{
				return parse_func_decl();
			}
		}
	}

	return new NExpressionStatement(*parse_expression());
}

NExpression * Parser::parse_expression(){
	std::cout << "parse_expression: " << peek().to_string() << std::endl;

	if(is_prefix_op()){
		return new NPrefixOp(peek(), *parse_expression());
	}

	NExpression * left = parse_atom();

	while(!eof()){
		if(is_infix_op()){
			left = parse_infix(left, 0);
		}else if(is_postfix_op()){
			// Note: After postfix operator there cannot be anything
			return new NPostfixOp(*left, peek());
		}else if(is_op(OP_LPAREN)){
			left = parse_func_call(left);
		}else{
			break;
		}
	}

	return left;
}

NExpression * Parser::parse_atom(){

	std::cout << "parse_atom: " << peek().to_string() << std::endl;

	Token current = peek();

	if(is_typeof(T_INT)){
		advance();
		return new NInt(current);
	}
	if(is_typeof(T_FLOAT)){
		advance();
		return new NFloat(current);
	}
	if(is_typeof(T_ID)){
		return parse_identifier();
	}
	if(is_typeof(T_STR)){
		advance();
		return new NString(current);
	}

	if(is_op(OP_LPAREN)){
		// Parse subexpression
		skip_op(OP_LPAREN, false, true);
		NExpression * expr = parse_expression();
		skip_op(OP_RPAREN, true, false);
		return expr;
	}

	if(is_typeof(T_KW)){
		switch(peek().kw()){
			case KW_IF:{
				return parse_if_expression();
				break;
			}
		}
	}

	unexpected_token();

	return nullptr;
}

NExpression * Parser::parse_infix(NExpression * left, int prec){
	if(is_infix_op()){
		Token t = peek();
		int right_prec = infix_precedence.at(t.op());
		if(right_prec > prec){
			advance();
			return parse_infix(
				new NInfixOp(*left, t, *parse_infix(parse_expression(), right_prec)),
				prec);
		}
	}
	return left;
}

NBlock * Parser::parse_block(const bool & allow_one_line){
	StatementList statements;
		
	if(!is_op(OP_LBRACE)){
		if(!allow_one_line){
			expected_error("operator `{`", "");
			return nullptr;
		}
		// Parse one-line block
		statements.push_back(parse_statement());
		return new NBlock(statements);
	}

	skip_op(OP_LBRACE, false, true);
	
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
		statements.push_back(parse_statement());
	}

	skip_op(OP_RBRACE, true, false);

	return new NBlock(statements);
}

NIdentifier * Parser::parse_identifier(){
	if(!is_typeof(T_ID)){
		expected_error("identifier", "");
		return nullptr;
	}

	NIdentifier * id = new NIdentifier(peek());
	advance();
	return id;
}

NVarDecl * Parser::parse_var_decl(){
	bool is_val = false;
	if(is_kw(KW_VAR)){
		skip_kw(KW_VAR, false, false);
	}else if(is_kw(KW_VAL)){
		skip_kw(KW_VAL, false, false);
		is_val = true;
	}

	NIdentifier * id = parse_identifier();

	NExpression * assign_expr = nullptr;
	if(is_op(OP_ASSIGN)){
		advance();
		assign_expr = parse_expression();
	}

	return new NVarDecl(is_val, *id, assign_expr);
}

NFuncDecl * Parser::parse_func_decl(){
	skip_kw(KW_FUNC, false, false);

	NIdentifier * id = parse_identifier();

	skip_op(OP_LPAREN, false, true);

	std::vector<Param> params;

	bool first = true;
	while(!eof()){
		if(is_op(OP_RPAREN)){
			break;
		}
		if(first){
			first = false;
		}else{
			skip_op(OP_COMMA);
		}
		params.push_back({ *parse_identifier() });
	}
	skip_op(OP_RPAREN, true, true);

	NBlock * body = parse_block();

	return new NFuncDecl(*id, params, *body);
}

NIfExpression * Parser::parse_if_expression(){
	ConditionList conditions;

	skip_kw(KW_IF, false, true);

	// Parse if
	NExpression * condition = parse_expression();
	NBlock * body = parse_block();
	ConditionStructure If(*condition, *body);

	conditions.push_back(If);

	while(!eof()){
		if(is_kw(KW_ELIF)){
			std::cout << "Parse elif" << std::endl;
			skip_kw(KW_ELIF, true, true);
			NExpression * condition = parse_expression();

			// TODO: Think about one-line if body

			NBlock * body = parse_block();
			ConditionStructure Elif(*condition, *body);
			conditions.push_back(Elif);
		}else{
			break;
		}
	}

	NBlock * Else = nullptr;
	if(is_kw(KW_ELSE)){
		skip_kw(KW_ELSE, true, true);
		Else = parse_block();
	}

	return new NIfExpression(conditions, Else);
}

NFuncCall * Parser::parse_func_call(NExpression * left){
	skip_op(OP_LPAREN, false, true);

	ExpressionList args;

	bool first = true;
	while(!eof()){
		if(is_op(OP_RPAREN)){
			break;
		}
		if(first){
			first = false;
		}else{
			skip_op(OP_COMMA);
		}
		args.push_back(parse_expression());
	}

	skip_op(OP_RPAREN, true, false);

	return new NFuncCall(*left, args);
}

////////////
// Errors //
////////////

void Parser::unexpected_token(){
	throw "Unexpected " + peek().to_string();
}

void Parser::expected_error(const std::string & expected, const std::string & given){
	throw "Expected "+ expected +", "+ (given.empty() ? peek().to_string() : given) +" given";
}