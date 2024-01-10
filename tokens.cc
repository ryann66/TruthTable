#include <queue>
#include <set>
#include <iostream>
#include <ctype.h>

#include "tokens.h"
#include "errorHandler.h"

using std::set;
using std::queue;
using std::ostream;
using std::string;

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
    os << printToken(t);
    return os;
}

string printToken(Token t) {
    switch (t.type) {
        case Variable:
            char r[2] = {t.opt, '\0'};
            return string(r);
        case OpenParen:
            return "(";
        case CloseParen:
            return ")";
        case Not:
            return "~";
        case And:
            return "^";
        case Or:
            return "v";
        case Implication:
            return "->";
        case Biconditional:
            return "<->";
    }
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