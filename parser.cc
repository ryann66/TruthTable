#include <queue>
#include <stack>

#include "tokens.h"
#include "parser.h"
#include "errorHandler.h"

using std::queue;
using std::stack;

// checks the given preposition (in reverse polish notation)
// to ensure that it's correct / interpretable
// returns true if it is uninterpretable
// else it returns false
// prints errors as it encounters them
bool programContainsError(queue<Token> prog);

queue<Token> parseToRPN(queue<Token>& tokenlist) {
    bool error = programContainsError(tokenlist);
    queue<Token> polish;
    stack<Token> shunt;
    while (!tokenlist.empty()) {
        Token next = tokenlist.front();
        tokenlist.pop();
        switch (next.type) {
            case Variable:
                polish.push(next);
                break;
            case Not:
            case OpenParen:
                shunt.push(next);
                if (tokenlist.front().type == CloseParen) {
                    printError("Empty parenthesis", next, tokenlist.front());
                    goto error;
                }
                break;
            case CloseParen:
            {
                if (shunt.empty()) {
                    printError("Unmatched ')'", next);
                    goto error;
                }
                Token t = shunt.top();
                // move internal expression into the output
                while ((t = shunt.top()).type != OpenParen) {
                    shunt.pop();
                    polish.push(t);
                    if (shunt.empty()) {
                        printError("Unmatched ')'", next);
                        goto error;
                    }
                }
                shunt.pop();
                // move any unary prefix operators into the output (only Not)
                while (!shunt.empty() && (t = shunt.top()).type == Not) {
                    shunt.pop();
                    polish.push(t);
                }
                break;
            }
            case And:
            case Or:
            case Implication:
            case Biconditional:
            {
                while (!shunt.empty() && shunt.top().type != OpenParen && shunt.top().opt > next.opt) {
                    polish.push(shunt.top());
                    shunt.pop();
                }
                shunt.push(next);
                break;
            }
        }
    }
    // clear any remaining operators
    while (!shunt.empty()) {
        if (shunt.top().type == OpenParen) {
            printError("Unmatched '('", shunt.top());
            goto error;
        }
        polish.push(shunt.top());
        shunt.pop();
    }
    if (error) goto error;

    return polish;
error:
    while (!polish.empty()) polish.pop();
    return polish;
}

bool programContainsError(queue<Token> prog) {
    return false;
}