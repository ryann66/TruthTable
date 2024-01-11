#include <queue>
#include <set>
#include <stack>
#include <map>
#include <iostream>

#include "interpreter.h"
#include "tokens.h"
#include "bitSequence.h"
#include "errorHandler.h"

using std::queue;
using std::set;
using std::stack;
using std::map;
using std::cout;
using std::endl;

size_t pow(size_t base, size_t power) {
    size_t ret = 1;
    while (power--) ret *= base;
    return ret;
}

bool printTruthTable(queue<Token> prop, set<char> vars) {
    // p = 00001111
    // q = 00110011
    // r = 01010101
    // (res) = ????????
    // run simd* instructions on all of these at the same time
    if (prop.empty()) return false;

    map<char, BitSequence> varMap;  // vars to values
    stack<BitSequence> stk;  // interpreting stack

    // add variable definitions
    size_t rows = pow(2, vars.size());
    size_t altlen = rows;
    for (char c : vars) {
        altlen /= 2;
        BitSequence bs(rows, altlen);
        varMap[c] = bs;
    }

    // interpret
    while (!prop.empty()) {
        switch (prop.front().type) {
        case Variable:
            stk.push(varMap[prop.front().opt]);
            break;
        case Not:
        {
            if (stk.empty()) {
                printError("Internal error: empty stack", prop.front());
                return false;
            }
            BitSequence bs(~stk.top());
            stk.pop();
            stk.push(bs);
            break;
        }
        case And:
        {
            if (stk.empty()) {
                printError("Internal error: empty stack", prop.front());
                return false;
            }
            BitSequence bs1(stk.top());
            stk.pop();
            if (stk.empty()) {
                printError("Internal error: empty stack", prop.front());
                return false;
            }
            BitSequence bs2(stk.top());
            stk.pop();
            stk.push(bs1 & bs2);
            break;
        }
        case Or:
        {
            if (stk.empty()) {
                printError("Internal error: empty stack", prop.front());
                return false;
            }
            BitSequence bs1(stk.top());
            stk.pop();
            if (stk.empty()) {
                printError("Internal error: empty stack", prop.front());
                return false;
            }
            BitSequence bs2(stk.top());
            stk.pop();
            stk.push(bs1 | bs2);
            break;
        }
        case Implication:
        {
            // A->B = ~AvB
            if (stk.empty()) {
                printError("Internal error: empty stack", prop.front());
                return false;
            }
            BitSequence rhs(stk.top());
            stk.pop();
            if (stk.empty()) {
                printError("Internal error: empty stack", prop.front());
                return false;
            }
            BitSequence lhs(~stk.top());
            stk.pop();
            stk.push(lhs | rhs);
            break;
        }
        case Biconditional:
        {
            if (stk.empty()) {
                printError("Internal error: empty stack", prop.front());
                return false;
            }
            BitSequence bs1(stk.top());
            stk.pop();
            if (stk.empty()) {
                printError("Internal error: empty stack", prop.front());
                return false;
            }
            BitSequence bs2(stk.top());
            stk.pop();
            stk.push(~(bs1 ^ bs2));
            break;
        }
        default:
            printError("Internal error: unexpected token '" + printToken(prop.front()) + "'", prop.front());
            return false;
        }
        prop.pop();
    }


    // TODO
    return false;
}