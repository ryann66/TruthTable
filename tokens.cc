#include <queue>
#include <set>
#include <iostream>
#include <ctype.h>

#include "tokens.h"
#include "errorHandler.h"

using std::set;
using std::queue;
using std::ostream;

queue<Token> tokenize(char* input, set<char>& variables) {
    queue<Token> ret;
    variables.clear();
    unsigned char len = 0; // char counter
    while (*input) {
        len++;
        switch (*input) {
            case '~': // not
                ret.emplace(Not, len);
                break;
            case '^': // and
                ret.emplace(And, len);
                break;
            case 'v': // or
            case 'V':
                ret.emplace(Or, len);
                break;
            case '-': // implication
                input++;
                if (input[0] != '>') goto error;
                ret.emplace(Implication, len);
                len++;
                break;
            case '<': // bicon
                input++;
                if (input[0] != '-' || input[1] != '>') goto error;
                ret.emplace(Biconditional, len);
                len += 2;
                input++;
                break;
            case '(':
                ret.emplace(OpenParen, len);
                break;
            case ')':
                ret.emplace(CloseParen, len);
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
                ret.emplace(*input, len);
                variables.insert(*input);
                break;
        }
        input++;
    }
    return ret;
error:
    printError("Unknown operator", len);
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

unsigned char tokenLength(Token t) {
    switch (t.type) {
    case Variable:
    case OpenParen:
    case CloseParen:
    case Not:
    case And:
    case Or:
        return 1;
    case Implication:
        return 2;
    case Biconditional:
        return 3;
    }
    return 0;
}