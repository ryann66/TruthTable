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

queue<Token> tokenize(const char* input, set<char>& variables, Style style) {
    queue<Token> ret;
    variables.clear();
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
    // todo
    return "";
}

unsigned char tokenLength(Token t) {
    if (t.type == Implication || t.style == Default && (t.type == And || t.type == Or)) return 2;
    if (t.type == Biconditional && t.style == Logical) return 3;
    return 1;
}