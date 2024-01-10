#pragma once

#include <queue>

#include "tokens.h"

using std::queue;

// parses the token list to reverse polish (postfix) notation
// consumes the queue tokenlist
// uses the shunting yard algorithm
// returns an empty queue on error
queue<Token> parseToRPN(queue<Token>& tokenlist);