#include <queue>
#include <stack>

#include "tokens.h"
#include "parser.h"

using std::queue;
using std::stack;
using std::cerr;
using std::endl;

queue<Token> parseToRPN(queue<Token>& tokenlist) {
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
                    cerr << "Empty parenthesis at characters " << (short int) next.loc << "-" 
                         << (short int) tokenlist.front().loc << endl;
                    goto error;
                }
                break;
            case CloseParen:
            {
                if (shunt.empty()) {
                    cerr << "Unmatched ')' at character " << (short int) next.loc << endl;
                    goto error;
                }
                Token t = shunt.top();
                // move internal expression into the output
                while ((t = shunt.top()).type != OpenParen) {
                    shunt.pop();
                    polish.push(t);
                    if (shunt.empty()) {
                        cerr << "Unmatched ')' at character " << (short int) next.loc << endl;
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
            cerr << "Unmatched '(' at character " << (short int) shunt.top().loc << endl;
            goto error;
        }
        polish.push(shunt.top());
        shunt.pop();
    }

    return polish;
error:
    while (!polish.empty()) polish.pop();
    return polish;
}