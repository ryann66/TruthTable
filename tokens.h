#pragma once

#include <queue>
#include <set>
#include <iostream>
#include <ctype.h>
#include <climits>

// Type of token
typedef enum : char {
    Variable, OpenParen, CloseParen, Not,
    And, Or, Xor, Implication, Biconditional
} Type;

// Style of token
typedef enum : char {
    Default, Logical, Boolean, CStyle
} Style;

// class for representing tokens
// Contains a token of given type, written in the given style
// If type is a variable, then opt is the letter
// Else if type is an operator, then opt is the precedence
// Else opt is unused
// loc is the 1 based index of the first character of the token in
// the input string
class Token {
public:
    Type type;
    Style style;
    unsigned char opt, loc;

    // assign type and autogen precedence
    Token(const Type t, const Style s, const unsigned char l) : type(t), loc(l), style(s) {
        switch (t) {
            case Not: // all unary operators
                opt = UCHAR_MAX;
                break;
            case Xor:
                opt = 5;
                break;
            case And:
                opt = 4;
                break;
            case Or:
                opt = 3;
                break;
            case Implication:
                opt = 2;
                break;
            case Biconditional:
                opt = 1;
                break;
        }
        // default leave opt unused
    }

    // type must be variable (assumed), assign var name to opt
    Token(const char v, const unsigned char l) : type(Variable), opt(v), loc(l) { }
};

std::ostream& operator<<(std::ostream& os, const Token);

std::string printToken(Token t);

// gets the length of the token (as a string)
unsigned char tokenLength(Token t);

// transforms the string input into a queue of tokens
// saves all of the input variable names into the set variables
// returns an empty queue on failure
std::queue<Token> tokenize(const char* input, std::set<char>& variables, Style style = Default);