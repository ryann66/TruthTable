#pragma once

#include <queue>
#include <set>
#include <iostream>
#include <ctype.h>

// Type of token
typedef enum {
    Variable, OpenParen, CloseParen, Not,
    And, Or, Implication, Biconditional
} Type;

// class for representing tokens
// Contains a token of given type
// If type is a variable, then opt is the letter
// Else if type is an operator, then opt is the precedence
// Else opt is unused
// loc is the 1 based index of the first character of the token in
// the input string
class Token {
public:
    Type type;
    char opt;
    unsigned char loc;

    // assign type and autogen precedence
    Token(const Type t, const unsigned char l) : type(t), loc(l) {
        switch (t) {
            case Not:
                opt = 5; // unary postfix gets no precedence
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

// transforms the string input into a queue of tokens
// saves all of the input variable names into the set variables
// returns an empty queue on failure
std::queue<Token> tokenize(char* input, std::set<char>& variables);