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
class Token {
    Type type;
    char opt;

public:
    // assign type and autogen precedence
    Token(const Type t) : type(t) {
        switch (t) {
            case Not:
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
    Token(const char v) : type(Variable), opt(v) { }

    friend std::ostream& operator<<(std::ostream& os, const Token);
};

// transforms the string input into a queue of tokens
// saves all of the input variable names into the set variables
// returns an empty queue on failure
std::queue<Token> tokenize(char* input, std::set<char>& variables);