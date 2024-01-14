#include <queue>
#include <set>
#include <iostream>
#include <ctype.h>

#include "tokens.h"
#include "errorHandler.h"
#include "tokenMatcher.h"

using std::set;
using std::queue;
using std::ostream;
using std::string;

// inserts the default operator type between all duplicate variable tokens
void insertDefaultOperator(queue<Token>& tokenlist, Type type, Style style);

queue<Token> tokenize(const char* input, set<char>& variables, Style style) {
    queue<Token> ret;
    setInputString(input);
    unsigned char len = 0;  // char counter
    while (*input) {
        TokenMatch res;
        if (matchToken(input, style, &res)) {
            // assorted token
            ret.emplace(res.type, res.style, len + 1);
            len += res.length;
            input += res.length;
        } else {
            // var
            if (!(isalpha(*input) || *input == '0' || *input == '1')) {
                goto error;
            }
            ret.emplace(*input, len);
            variables.insert(*input);
            input++;
            len++;
        }
    }

    if (style == Boolean) insertDefaultOperator(ret, And, Boolean);
    return ret;
error:
    printError("Unknown operator", len + 1);
    while (!ret.empty()) ret.pop();
    return ret;
}

ostream& operator<<(ostream& os, const Token t) {
    os << printToken(t);
    return os;
}

string printToken(Token t) {
    switch (t.type) {
    case Not:
        switch (t.style) {
        case Default:
            return "!";
        case Logical:
        case Boolean:
            return "-";
        case CStyle:
            return "~";
        }
    case And:
        switch (t.style) {
        case Default:
            return "&&";
        case Logical:
            return "^";
        case Boolean:
            return "*";
        case CStyle:
            return "&";
        }
    case Or:
        switch (t.style) {
        case Default:
            return "||";
        case Logical:
            return "v";
        case Boolean:
            return "+";
        case CStyle:
            return "|";
        }
    case Xor:
        switch (t.style) {
        case Logical:
            return "+";
        case Default:
        case Boolean:
        case CStyle:
            return "^";
        }
    case Implication:
        if (t.style == Default || t.style == Logical)
            return "->";
    case Biconditional:
        switch (t.style) {
        case Logical:
            return "<->";
        case Default:
        case Boolean:
        case CStyle:
            return "=";
        }
    case OpenParen:
        switch (t.style) {
        case Default:
            return "(";
        case Boolean:
            return "[";
        case CStyle:
            return "{";
        }
    case CloseParen:
        switch (t.style) {
        case Default:
            return ")";
        case Boolean:
            return "]";
        case CStyle:
            return "}";
        }
    }
    printError("Internal error: illegal token state");
    return "";
}

unsigned char tokenLength(Token t) {
    if (t.type == Implication || t.style == Default && (t.type == And || t.type == Or)) return 2;
    if (t.type == Biconditional && t.style == Logical) return 3;
    return 1;
}

void insertDefaultOperator(queue<Token>& tokenlist, Type type, Style style) {
    if (tokenlist.empty()) return;
    queue<Token> tmp;
    tokenlist.swap(tmp);
    Type last = tmp.front().type;
    tokenlist.push(tmp.front());
    tmp.pop();
    while (!tmp.empty()) {
        if (last == Variable && tmp.front().type == Variable) {
            tokenlist.emplace(type, style, tmp.front().loc);
        }
        last = tmp.front().type;
        tokenlist.push(tmp.front());
        tmp.pop();
    }

}