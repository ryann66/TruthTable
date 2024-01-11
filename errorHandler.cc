#include <string>
#include <cstdio>
#include <iostream>

#include "errorHandler.h"
#include "tokens.h"
extern "C" {
    #include "bufferedErrorStream.h"
}

#define ESCAPE "\033"
#define RESET ESCAPE "[0m"
#define EMPH ESCAPE "[35m"

using std::string;
using std::cerr;
using std::endl;

string input_string;

void setInputString(const char* inputString) {
    input_string = inputString;
}

void printError(const string& message) {
    printString(message.c_str());
    printRepeated('\n', 1);
    flush();
}

void printError(const char* message, unsigned char loc) {
    // print line 1
    int len = printString(message);
    len += printString(" at character ");
    len += printNum(loc);
    len += printString(":  ");
    printString(input_string.c_str());
    printString("\n");
    len += loc;
    // print error squiggles
    printRepeated(' ', len - 1);
    printString(EMPH);
    printRepeated('~', (unsigned int) 1);
    printString(RESET);
    printRepeated('\n', 1);
    flush();
}

void printError(const char* message, unsigned char beg, unsigned char end) {
    if (beg > end) return;
    // print line 1
    int len = printString(message);
    len += printString(" at characters ");
    len += printNum(beg);
    len += printRepeated('-', 1u);
    len += printNum(end);
    len += printString(":  ");
    printString(input_string.c_str());
    printRepeated('\n', 1);
    len += beg;
    // print error squiggles
    printRepeated(' ', len - 1);
    printString(EMPH);
    printRepeated('~', (unsigned int) end - beg + 1);
    printString(RESET);
    printRepeated('\n', 1);
    flush();
}

void printError(const char* message, const Token& loc) {
    unsigned char len = tokenLength(loc);
    if (len) {
        if (len == 1) printError(message, loc.loc);
        else printError(message, loc.loc, loc.loc + len - 1);
    }
}

void printError(const char* message, const Token& beg, const Token& end) {
    if (beg.loc > end.loc) return;
    unsigned char len = tokenLength(end);
    printError(message, beg.loc, end.loc + len - 1);
}

void printError(const string& message, const Token& loc) {
    printError(message.c_str(), loc);
}

void printError(const string& message, const Token& beg, const Token& end) {
    printError(message.c_str(), beg, end);
}