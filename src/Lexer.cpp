#include "Lexer.h"

Lexer::Lexer(const std::fstream & input){
	std::stringstream ss;
	ss << input.rdbuf();
	script = ss.str();

	index = 0;
	line = 0;
	column = 0;
}

char Lexer::peek(){
	return script[index];
}

char Lexer::advance(){
	index++;
	if(peek() == '\n'){
		line++;
		column = 0;
	}else{
		column++;
	}
	return peek();
}

bool Lexer::eof(){
	return peek() == '\0';
}

bool Lexer::skip(const char & c){
	return peek() == '\t' || peek() == ' ';
}

bool Lexer::is_newline(const char & c){
	return peek() == '\n';
}

bool Lexer::is_digit(const char & c){
	return peek() >= '0' && peek() <= '9';
}

bool Lexer::is_id_first(const char & c){
	return (peek() >= 'a' && peek() <= 'z')
		|| (peek() >= 'A' && peek() <= 'Z')
		|| (peek() == '_');
}

bool Lexer::is_id(const char & c){
	return is_id_first(c) || is_digit(c);
}

bool Lexer::is_quote(const char & c){
	return c == '"' || c == '\'' || c == '`';
}

void Lexer::add_token(const TokenType & type, const std::string & val){
	Token t(type, val);

	t.pos.line = line;
	t.pos.column = column;
	t.pos.endIndex = index;

	tokens.push_back(t);
}

void Lexer::add_token(const Operator & op){
	Token t(op);

	t.pos.line = line;
	t.pos.column = column;
	t.pos.endIndex = index;

	tokens.push_back(t);
}

void Lexer::add_token(const Keyword & kw){
	Token t(kw);

	t.pos.line = line;
	t.pos.column = column;
	t.pos.endIndex = index;

	tokens.push_back(t);
}

void Lexer::add_token(const TokenType & type){
	Token t(type);

	t.pos.line = line;
	t.pos.column = column;
	t.pos.endIndex = index;

	tokens.push_back(t);
}

TokenStream Lexer::lex(){

	std::cout << "Lexing script:\n" << script << std::endl;

	while(!eof()){
		if(skip(peek())){
			advance();
		}else if(is_newline(peek())){
			add_token(T_NL);
			advance();
		}else if(is_digit(peek())){
			std::string num(1, peek());
			while(is_digit(advance())){
				num += peek();
			}
			if(peek() == '.'){
				do{
					num += peek();
				}
				while(is_digit(advance()));
				add_token(T_FLOAT, num);
				continue;
			}
			add_token(T_INT, num);
		}else if(is_id_first(peek())){
			std::string id(1, peek());
			while(is_id(advance())){
				id += peek();
			}

			Keyword kw = str_to_kw(id);

			if(kw < KW_MAX){
				add_token(kw);
			}else{
				add_token(T_ID, id);
			}
		}else if(is_quote(peek())){
			const char quote = peek();
			advance();
			std::string str = "";
			while(!eof()){
				if(peek() == quote){
					break;
				}
				str += peek();
				advance();
			}
			if(eof()){
				unexpected_eof_error();
			}
			if(peek() != quote){
				unexpected_error();
			}
			add_token(T_STR, str);
			advance();
		}else{
			switch(peek()){
				case '=':{
					add_token(OP_ASSIGN);
					advance();
					break;
				}
				case '+':{
					add_token(OP_ADD);
					advance();
					break;
				}
				case '-':{
					add_token(OP_SUB);
					advance();
					break;
				}
				case '*':{
					add_token(OP_MUL);
					advance();
					break;
				}
				case '/':{
					add_token(OP_DIV);
					advance();
					break;
				}
				case '%':{
					add_token(OP_MOD);
					advance();
					break;
				}
				case ';':{
					add_token(OP_SEMICOLON);
					advance();
					break;
				}
				case '(':{
					add_token(OP_LPAREN);
					advance();
					break;
				}
				case ')':{
					add_token(OP_RPAREN);
					advance();
					break;
				}
				case '{':{
					add_token(OP_LBRACE);
					advance();
					break;
				}
				case '}':{
					add_token(OP_RBRACE);
					advance();
					break;
				}
				case ',':{
					add_token(OP_COMMA);
					advance();
					break;
				}
				default:{
					unexpected_error();
				}
			}
		}
	}

	add_token(T_EOF);

	return tokens;
}

void Lexer::unexpected_error(){
	throw "Unexpected token `"+ std::string(1, peek()) +
		  "` at "+ std::to_string(line) +":"+ std::to_string(column);
}

void Lexer::unexpected_eof_error(){
	throw "Unexpected end of file";
}