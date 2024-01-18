#pragma once

#include <vector>
#include <queue>
#include <set>

#include "bitSequence.h"
#include "tokens.h"

using std::vector;
using std::queue;
using std::set;
using std::pair;

// prints the full truth table of the given propositions with the
// given input variables
// handles a maximum of 10 propositions at once
// returns a vector form of the truth table
// rows is an output parameter of the number of rows in the resultant table
// in case of error, returns an empty truth table
vector<pair<char, BitSequence>> interpretPropositions (vector<queue<Token>> propositions, set<char> vars, size_t* rows);