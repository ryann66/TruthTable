#pragma once

#include <queue>
#include <set>

#include "tokens.h"

using std::queue;
using std::set;

// prints the full truth table of the given proposition with the
// given input variables
// returns false on error, else true
bool printTruthTable(queue<Token> prop, set<char> vars);