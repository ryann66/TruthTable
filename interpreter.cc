#include <queue>
#include <set>
#include <stack>
#include <map>
#include <iostream>

#include "interpreter.h"
#include "tokens.h"
#include "bitSequence.h"
#include "errorHandler.h"

#define V_SEP " | "
#define OUTPUT_NAME "out"
#define H_BAR "-"
#define H_BAR_V_SEP "-+-"
#define H_BAR_OUTPUT "---"

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

    // remove special variables from vars
    bool zero = vars.erase('0'), one = vars.erase('1');
    if (vars.empty()) {
        printError("No variables detected!");
        return false;
    }

    // add variable definitions
    size_t rows = pow(2, vars.size());
    {
        size_t altlen = rows;
        for (char c : vars) {
            altlen /= 2;
            BitSequence bs(rows, altlen);
            varMap[c] = bs;
        }
    }

    // add special variables to var mapping
    if (zero & one) {
        BitSequence bs(rows, 0);
        varMap['0'] = bs;
        varMap['1'] = ~bs;
    } else {
        if (zero) varMap['0'] = BitSequence(rows, 0);
        if (one) varMap['1'] = ~BitSequence(rows, 0);
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
    if (stk.empty()) {
        printError("Internal error: empty stack", prop.front());
        return false;
    }
    if (stk.size() > 1) {
        printError("Internal error: more than one element remaining on stack");
        return false;
    }

    // generate header and array of sequences
    size_t nCols = vars.size() + 1;
    BitSequence* seqs = new BitSequence[nCols];
    {
        BitSequence* tmp = seqs;
        for (char c : vars) {
            *tmp = varMap[c];
            tmp++;
            cout << c << V_SEP;
        }
        *tmp = stk.top();
        stk.pop();
        cout << OUTPUT_NAME << endl;
    }

    // print separator
    for (size_t i = 0; i < vars.size(); i++) {
        cout << H_BAR << H_BAR_V_SEP;
    }
    cout << H_BAR_OUTPUT << endl;

    // print table
    for (size_t i = 0; i < rows; i++) {
        cout << seqs[0][i];
        for (size_t j = 1; j < nCols; j++) {
            cout << V_SEP << seqs[j][i];
        }
        cout << endl;
    }

    return true;
}