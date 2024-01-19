#pragma once

#include <vector>

#include "bitSequence.h"

using std::vector;
using std::pair;

typedef enum : char {
    All, Highlight, Different
} PrintStyle;

// prints the table in the given style
// Note: prints using c-style printf for speed
void printTable(const vector<pair<char, BitSequence>>& table, size_t nRows, PrintStyle style);