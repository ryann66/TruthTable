#include <queue>
#include <set>
#include <iostream>
#include <ctype.h>

#include "tokens.h"

using std::set;
using std::queue;
using std::cerr;
using std::endl;
using std::ostream;

queue<Token> tokenize(char* input, set<char>& variables) {
    queue<Token> ret;
    variables.clear();
    char* orig = input;
    while (*input) {
        switch (*input) {
            case '~': // not
                ret.emplace(Not);
                break;
            case '^': // and
                ret.emplace(And);
                break;
            case 'v': // or
            case 'V':
                ret.emplace(Or);
                break;
            case '-': // implication
                input++;
                if (input[0] != '>') goto error;
                ret.emplace(Implication);
                break;
            case '<': // bicon
                input++;
                if (input[0] != '-' || input[1] != '>') goto error;
                ret.emplace(Biconditional);
                input++;
                break;
            case '(':
                ret.emplace(OpenParen);
                break;
            case ')':
                ret.emplace(CloseParen);
                break;
            case ' ': // skip (whitespace)
            case '\r':
            case '\n':
                break;
            default: // variable
                if (!isalpha(*input)) {
                    input++;
                    goto error;
                }
                ret.emplace(*input);
                variables.insert(*input);
                break;
        }
        input++;
    }
    return ret;
error:
    cerr << "Unknown operator at character " << (input - orig) << endl;
    while (!ret.empty()) ret.pop();
    variables.clear();
    return ret;
}

ostream& operator<<(ostream& os, const Token t) {
    switch (t.type) {
        case Variable:
            os << t.opt;
            break;
        case OpenParen:
            os << "(";
            break;
        case CloseParen:
            os << ")";
            break;
        case Not:
            os << "~";
            break;
        case And:
            os << "^";
            break;
        case Or:
            os << "v";
            break;
        case Implication:
            os << "->";
            break;
        case Biconditional:
            os << "<->";
            break;
    }
    return os;
}