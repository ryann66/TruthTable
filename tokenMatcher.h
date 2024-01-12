#pragma once

#include "tokens.h"

// struct for returning the results of a token match search
typedef struct token_match {
    Type type;
    Style style;
    size_t length;
} TokenMatch;

// attempts to match the given string with a token in the given style set
//   str: char pointer to the string to read from
//   style: the style to read symbols in (see tokens.txt)
//   ret: output parameter to return the match in
// returns true on successful match, else false
bool matchToken (const char* str, Style style, TokenMatch* ret);