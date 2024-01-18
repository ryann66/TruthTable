#include <vector>
#include <queue>
#include <set>
#include <stack>
#include <map>
#include <cmath>

#include "interpreter.h"
#include "tokens.h"
#include "bitSequence.h"
#include "errorHandler.h"

#define N_DIGITS(x) ((unsigned char)((x ? floor(log10(abs((float) (x)))) : 1) + 1))

using std::vector;
using std::queue;
using std::set;
using std::stack;
using std::map;
using std::pair;

size_t pow(size_t base, size_t power) {
    size_t ret = 1;
    while (power--) ret *= base;
    return ret;
}

// evaluates (and consumes) the given proposition in the given variable environment (varMap)
BitSequence evaluate (queue<Token>& prop, const map<char, BitSequence>& varMap);

vector<pair<char, BitSequence>> interpretPropositions (vector<queue<Token>> props, set<char> vars, size_t* rows) {
    if (props.empty() || props.size() > 10) return vector<pair<char, BitSequence>>();
    map<char, BitSequence> varMap;  // vars to values

    // build var map
    {
        // remove special variables from vars
        bool zero = vars.erase('0'), one = vars.erase('1');
        *rows = pow(2, vars.size());
        
        // add special variables to var mapping
        if (zero & one) {
            BitSequence bs(*rows, 0);
            varMap['0'] = bs;
            varMap['1'] = ~bs;
        } else {
            if (zero) varMap['0'] = BitSequence(*rows, 0);
            if (one) varMap['1'] = ~BitSequence(*rows, 0);
        }

        // add normal variable definitions
        {
            size_t altlen = *rows;
            for (char c : vars) {
                altlen /= 2;
                BitSequence bs(*rows, altlen);
                varMap[c] = bs;
            }
        } 
    }
    
    // add variables to table
    vector<pair<char, BitSequence>> res;
    for (char c : vars) {
        res.emplace_back(c, varMap[c]);
    }

    // evaluate props and add to table
    for (int i = 0; i < props.size(); i++) {
        res.emplace_back(((i + 1) % 10) + 48, evaluate(props[i], varMap));
    }

    return res;
}

BitSequence evaluate (queue<Token>& prop, const map<char, BitSequence>& varMap) {
    stack<BitSequence> stk;

    while (!prop.empty()) {
        switch (prop.front().type) {
        case Variable:
            stk.push(const_cast<map<char, BitSequence>&>(varMap)[prop.front().opt]);
            break;
        case Not:
        {
            BitSequence bs(~stk.top());
            stk.pop();
            stk.push(bs);
            break;
        }
        case And:
        {
            BitSequence bs1(stk.top());
            stk.pop();
            BitSequence bs2(stk.top());
            stk.pop();
            stk.push(bs1 & bs2);
            break;
        }
        case Or:
        {
            BitSequence bs1(stk.top());
            stk.pop();
            BitSequence bs2(stk.top());
            stk.pop();
            stk.push(bs1 | bs2);
            break;
        }
        case Xor:
        {
            BitSequence bs1(stk.top());
            stk.pop();
            BitSequence bs2(stk.top());
            stk.pop();
            stk.push(bs1 ^ bs2);
            break;
        }
        case Implication:
        {
            // A->B = ~AvB
            BitSequence rhs(stk.top());
            stk.pop();
            BitSequence lhs(~stk.top());
            stk.pop();
            stk.push(lhs | rhs);
            break;
        }
        case Biconditional:
        {
            BitSequence bs1(stk.top());
            stk.pop();
            BitSequence bs2(stk.top());
            stk.pop();
            stk.push(~(bs1 ^ bs2));
            break;
        }
        }
        prop.pop();
    }

    BitSequence bs(stk.top());
    return bs;
}