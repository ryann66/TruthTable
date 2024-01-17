#include <vector>
#include <queue>
#include <set>
#include <stack>
#include <map>
#include <cstdio>
#include <cmath>

#include "interpreter.h"
#include "tokens.h"
#include "bitSequence.h"
#include "errorHandler.h"

#define V_SEP " | "
#define H_BAR "-"
#define FILLER " "
#define H_BAR_V_SEP "-+-"
#define NEWLINE "\n"

#define N_DIGITS(x) ((unsigned char)((x ? floor(log10(abs((float) (x)))) : 1) + 1))

using std::vector;
using std::queue;
using std::set;
using std::stack;
using std::map;

size_t pow(size_t base, size_t power) {
    size_t ret = 1;
    while (power--) ret *= base;
    return ret;
}

// evaluates (and consumes) the given proposition in the given variable environment (varMap)
BitSequence evaluate (queue<Token>& prop, const map<char, BitSequence>& varMap);

// prints a truth table with the given variables and results (numbered 1-n)
// each BitSequence must have the specified number of rows
void printTruthTable (const map<char, BitSequence>& vars, const vector<BitSequence>& results, size_t nRows);

// prints out each of the given results
// each result must have a length of 1
void printStaticEvalResults (const vector<BitSequence>& results);

bool interpretPropositions (vector<queue<Token>> props, set<char> vars) {
    if (props.empty()) return false;
    map<char, BitSequence> varMap;  // vars to values
    size_t rows;  // length of sequences

    // build var map
    {
        // remove special variables from vars
        bool zero = vars.erase('0'), one = vars.erase('1');
        rows = pow(2, vars.size());
        
        // add special variables to var mapping
        if (zero & one) {
            BitSequence bs(rows, 0);
            varMap['0'] = bs;
            varMap['1'] = ~bs;
        } else {
            if (zero) varMap['0'] = BitSequence(rows, 0);
            if (one) varMap['1'] = ~BitSequence(rows, 0);
        }

        // add normal variable definitions
        {
            size_t altlen = rows;
            for (char c : vars) {
                altlen /= 2;
                BitSequence bs(rows, altlen);
                varMap[c] = bs;
            }
        } 
    }
    
    // interpret everything
    vector<BitSequence> res;
    for (int i = 0; i < props.size(); i++) {
        res.push_back(evaluate(props[i], varMap));
    }
    
    // check if evaluating or building truth table
    if (vars.empty()) {
        printStaticEvalResults(res);
    } else {
        printTruthTable(varMap, res, rows);
    }

    return true;
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

void printTruthTable (const map<char, BitSequence>& vars, const vector<BitSequence>& results, size_t nRows) {
    // generate header and array of sequences
    size_t nCols = vars.size() + results.size();
    vector<BitSequence> seqs;
    seqs.reserve(nCols);
    vector<unsigned char> lens;
    lens.reserve(nCols);
    int index = 0;
    for (auto& pair : vars) {
        printf("%c" V_SEP, pair.first);
        seqs.push_back(pair.second);
        lens.push_back(1);
        index++;
    }
    for (int i = 0; i < results.size() - 1; i++) {
        printf("%i" V_SEP, i + 1);
        seqs.push_back(results[i]);
        lens.push_back(N_DIGITS(i + 1));
        index++;
    }
    printf("%llu" NEWLINE, results.size());
    seqs.push_back(results[results.size() - 1]);
    lens.push_back(N_DIGITS(results.size()));

    // print separator
    for (size_t i = 0; i < nCols - 1; i++) {
        for (int j = 0; j < lens[i]; j++)
            printf(H_BAR);
        printf(H_BAR_V_SEP);
        lens[i]--;
    }
    for (int j = -1; j < lens[nCols - 1]; j++) {
        printf(H_BAR);
    }
    printf(NEWLINE);
    lens[nCols - 1]--;

    // print table
    for (size_t i = 0; i < nRows; i++) {
        printf("%hhu", seqs[0][i]);
        for (size_t j = 1; j < nCols; j++) {
            printf(V_SEP);
            for (int k = 0; k < lens[j]; k++) printf(FILLER);
            printf("%hhu", seqs[j][i]);
        }
        printf(NEWLINE);
    }
}

void printStaticEvalResults (const vector<BitSequence>& results) {
    for (int i = 0; i < results.size(); i++) {
        printf("%hhu" NEWLINE, results[i][0]);
    }
}