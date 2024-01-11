#include <queue>
#include <stack>

#include "tokens.h"
#include "parser.h"
#include "errorHandler.h"

using std::queue;
using std::stack;
using std::string;

// Simplified enumeration of different types of operators that can be parsed
typedef enum {
    BinaryOperator, UnaryOperator /* prefix */, Value, Open, Close
} SimplifiedType;

// function to determine the simple type of all tokens
// Update when adding new token types to ensure class continues to function
SimplifiedType categorizeToken(Token t);

// checks that the given token list forms a valid program
// prints out errors as found
// returns false if error found, else true
bool invalidProgram(queue<Token>& tokenlist);

queue<Token> parseToRPN(queue<Token>& tokenlist) {
    if (tokenlist.empty()) return tokenlist;
    queue<Token> polish, orig(tokenlist);
    stack<Token> shunt;
    while (!tokenlist.empty()) {
        Token next = tokenlist.front();
        tokenlist.pop();
        switch (categorizeToken(next)) {
            case Value:
                polish.push(next);
                break;
            case UnaryOperator:
            case Open:
                shunt.push(next);
                break;
            case Close:
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
                break;
            }
            case BinaryOperator:
            {
                while (!shunt.empty() && shunt.top().type != OpenParen && shunt.top().opt >= next.opt) {
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

    if (invalidProgram(orig)) goto error;

    return polish;
error:
    while (!polish.empty()) polish.pop();
    return polish;
}

SimplifiedType categorizeToken(Token t) {
    switch (t.type) {
    case Variable:
        return Value;
    case OpenParen:
        return Open;
    case CloseParen:
        return Close;
    case Not:
        return UnaryOperator;
    case And:
    case Or:
    case Implication:
    case Biconditional:
        return BinaryOperator;
    }
    return Close;
}

// Follows a predefined set of rules, represented below in a CFG style
// S -> start
// E -> end
// v -> variable
// * -> binary operator
// ~ -> prefix operator
// ( -> open paren
// ) -> close paren
///////////////////////
// S -> ~, v, (
// v -> ), *, E
// * -> v, (, ~
// ~ -> (, v, ~
// ( -> (, v, ~
// ) -> ), *, E
// Must end with E, else error
bool invalidProgram(queue<Token>& tokenlist) {
    if (tokenlist.empty()) return true;
    bool buggy = false;
    Token prev = tokenlist.front();
    tokenlist.pop();
    SimplifiedType prevType = categorizeToken(prev);
    
    //  bootload start (Close cannot appear because it is caught in parsing)
    if (prevType == BinaryOperator) {
        printError("Leading binary operator '" + printToken(prev) + "'", prev);
        buggy = true;
    }
    
    while (!tokenlist.empty()) {
        Token next = tokenlist.front();
        SimplifiedType nextType = categorizeToken(next);
        tokenlist.pop();

        // main logic check
        switch (prevType) {
        case BinaryOperator:
            if (nextType == Close) {
                printError("Binary operator '" + printToken(prev) + "' expected an expression", prev, next);
                buggy = true;
            } else if (nextType == BinaryOperator) {
                printError("Binary operators '" + printToken(prev) + "' and '" + printToken(next) + "' expected an expression", prev, next);
                buggy = true;
            }
            break;
        case UnaryOperator:
            if (nextType == Close || nextType == BinaryOperator) {
                printError("Unary operator '" + printToken(prev) + "' expected an expression", prev, next);
                buggy = true;
            }
            break;
        case Value:
        case Close:
            if (nextType == Value || nextType == Open) {
                printError("Missing operator between expressions", prev, next);
                buggy = true;
            } else if (nextType == UnaryOperator) {
                printError("Illegal suffix operator '" + printToken(next) + "' used", next);
                buggy = true;
            }
            break;
        case Open:
            // cannot be followed by close (parsing guarantee)
            if (nextType == BinaryOperator) {
                printError("Binary operator '" + printToken(prev) + "' expected an expression", prev, next);
                buggy = true;
            }
            break;
        }

        prev = next;
        prevType = nextType;
    }

    // check end (open cannot appear because it is caught in parsing)
    switch (prevType) {
    case BinaryOperator:
        printError("Trailing binary operator '" + printToken(prev) + "'", prev);
        buggy = true;
        break;
    case UnaryOperator:
        printError("Trailing unary operator '" + printToken(prev) + "'", prev);
        buggy = true;
        break;
    }

    return buggy;
}