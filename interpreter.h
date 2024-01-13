#pragma once

#include <vector>
#include <queue>
#include <set>

#include "tokens.h"

using std::vector;
using std::queue;
using std::set;

// prints the full truth table of the given propositions with the
// given input variables
// returns false on error, else true
bool interpretPropositions (vector<queue<Token>> propositions, set<char> vars);