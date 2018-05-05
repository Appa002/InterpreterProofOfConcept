//
// Created by a_mod on 04.05.2018.
//

#ifndef COMPILER_TOKEN_H
#define COMPILER_TOKEN_H

#include <string>

// (5 + (3 + 4))

enum class TokenType : int {
    CallExpression,
    NumberLiteral,
    Operator,
    Undefined
};


struct Token {
    std::string Value;
    TokenType Type;
    Token() : Value(""), Type(TokenType::Undefined) {};
    Token(std::string val, TokenType type) : Value(val), Type(type) {};
};


#endif //COMPILER_TOKEN_H
