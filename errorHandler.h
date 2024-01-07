#pragma once

#include <string>

using std::string;

// sets the input string for all errors to be printed against
void setInputString(const char* inputString);

// prints an error message, highlighting where the error occured in the original input string
void printError(string message, unsigned char loc);

// prints an error message, highlighting where the error occured in the original input string
void printError(string message, unsigned char beg, unsigned char end);