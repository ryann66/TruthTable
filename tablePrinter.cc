#include <vector>
#include <stack>
#include <cstdio>
#include <cctype>

#include "tablePrinter.h"
#include "terminalSequences.h"
#include "bitSequence.h"

#define V_SEP " | "
#define VH_SEP "-+-"
#define H_SEP "-"
#define NEWLINE "\n"

using std::vector;
using std::pair;
using std::stack;

// returns true if the given table column is a proposition
inline bool isProposition(pair<char, BitSequence> col);

// prints the table
void printFullTable(const vector<pair<char, BitSequence>>& table, size_t nRows);

// prints the entire table, highlights rows that are different
void printTableHighlight(const vector<pair<char, BitSequence>>& table, size_t nRows);

// prints only the rows of the table that are different
void printTableDiff(const vector<pair<char, BitSequence>>& table, size_t nRows);

void printHeader(const vector<pair<char, BitSequence>>& table);

// returns a bitsequence with 1 representing that the row contains differences
// and 0 representing that the row is uniform
BitSequence rowDiff(const vector<pair<char, BitSequence>>& table);

void printTable(const vector<pair<char, BitSequence>>& table, size_t nRows, PrintStyle style) {
    switch (style) {
        case All:
            printFullTable(table, nRows);
            break;
        case Different:
            printTableDiff(table, nRows);
            break;
        case Highlight:
            printTableHighlight(table, nRows);
            break;
    }
}

inline bool isProposition(pair<char, BitSequence> col) {
    return isdigit(col.first);
}

void printFullTable(const vector<pair<char, BitSequence>>& table, size_t nRows) {
    printHeader(table);

    // print rows
    for (size_t r = 0; r < nRows; r++) {
        // print cols
        auto iter = table.begin();
        printf("%hhu", iter->second[r]);
        iter++;
        for (; iter != table.end(); iter++) {
            printf(V_SEP "%hhu", iter->second[r]);
        }
        printf(NEWLINE);
    }
}

void printTableHighlight(const vector<pair<char, BitSequence>>& table, size_t nRows) {
    printHeader(table);
    BitSequence diff(rowDiff(table));
    
    // print rows
    for (size_t r = 0; r < nRows; r++) {
        // print cols
        auto iter = table.begin();
        if (diff[r]) {
            printf(EMPH "%hhu" RESET, iter->second[r]);
        } else {
            printf("%hhu", iter->second[r]);
        }
        iter++;
        for (; iter != table.end(); iter++) {
            if (diff[r]) {
                printf(V_SEP EMPH "%hhu" RESET, iter->second[r]);
            } else {
                printf(V_SEP "%hhu", iter->second[r]);
            }
        }
        // print cols highlight
        printf(NEWLINE);
    }
}

void printTableDiff(const vector<pair<char, BitSequence>>& table, size_t nRows) {
    BitSequence diff(rowDiff(table));
    if (!((bool)diff)) {
        printf("No differences in table" NEWLINE);
        return;
    }
    printHeader(table);
    
    // print rows
    for (size_t r = 0; r < nRows; r++) {
        if (!diff[r]) continue;
        // print cols
        auto iter = table.begin();
        printf("%hhu", iter->second[r]);
        iter++;
        for (; iter != table.end(); iter++) {
            printf(V_SEP "%hhu", iter->second[r]);
        }
        printf(NEWLINE);
    }
}

void printHeader(const vector<pair<char, BitSequence>>& table) {
    auto iter = table.begin();
    printf("%c", iter->first);
    iter++;
    for(; iter != table.end(); iter++) {
        printf(V_SEP "%c", iter->first);
    }
    printf(NEWLINE H_SEP);
    for (size_t i = 1; i < table.size(); i++) {
        printf(VH_SEP H_SEP);
    }
    printf(H_SEP NEWLINE);
}

BitSequence rowDiff(const vector<pair<char, BitSequence>>& table) {
    auto iter = table.begin();
    
    // get first prop (at least one must be a prop)
    while (!isProposition(*iter)) iter++;
    BitSequence anda(iter->second), ora((iter)->second);
    iter++;

    for (; iter != table.end(); iter++) {
        if (!isProposition(*iter)) continue;
        anda &= iter->second;
        ora |= iter->second;
    }

    return anda ^ ora;
}