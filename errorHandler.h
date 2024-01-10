#pragma once

#include <string>

#include "tokens.h"

using std::string;

// sets the input string for all errors to be printed against
void setInputString(const char* inputString);

// prints an error message, highlighting where the error occured in the original input string
void printError(const char* message, unsigned char loc);

// prints an error message, highlighting where the error occured in the original input string
void printError(const char* message, unsigned char beg, unsigned char end);

// prints an error message, highlighting the given token
void printError(const char* message, const Token& loc);

// prints an error message, highlighting the input string enclosed between the tokens
void printError(const char* message, const Token& beg, const Token& end);

// prints an error message, highlighting the given token
inline void printError(string& message, const Token& loc);

// prints an error message, highlighting the input string enclosed between the tokens
inline void printError(string& message, const Token& beg, const Token& end);