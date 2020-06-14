#ifndef LEXER_H
#define LEXER_H

#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>

#include "Token.h"

class Lexer {
public:
    Lexer(const std::fstream & input);
    virtual ~Lexer() = default;

    char peek();
    char advance();

    TokenStream lex();

private:
    std::string script;
    TokenStream tokens;

    void add_token(const TokenType & type, const std::string & val);
    void add_token(const Operator & op);
    void add_token(const Keyword & kw);
    void add_token(const TokenType & type);

    uint32_t index;
    uint32_t line;
    uint32_t column;

    // Determinators
    bool skip(const char & c);
    bool is_newline(const char & c);
    bool is_digit(const char & c);
    bool is_id_first(const char & c);
    bool is_id(const char & c);

    // Errors
    void unexpected_error();
};

#endif