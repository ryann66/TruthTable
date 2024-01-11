#include <queue>
#include <set>
#include <stack>
#include <map>
#include <iostream>

#include "interpreter.h"
#include "tokens.h"
#include "bitSequence.h"

using std::queue;
using std::set;
using std::stack;
using std::map;
using std::cout;
using std::endl;

bool printTruthTable(queue<Token> prop, set<char> vars) {
    // p = 00001111
    // q = 00110011
    // r = 01010101
    // (res) = ????????
    // run simd* instructions on all of these at the same time

    map<char, BitSequence> varMap;//vars to values
    stack<BitSequence> stk;//stack

    // TODO
    return false;
}