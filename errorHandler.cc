#include <string>
#include <cstdio>
#include <iostream>

#include "errorHandler.h"
extern "C" {
    #include "bufferedErrorStream.h"
}

using std::string;
using std::cerr;
using std::endl;

string input_string;

void setInputString(const char* inputString) {
    input_string = inputString;
}

void printError(string message, unsigned char loc) {
    // print line 1
    int len = printString(message.c_str());
    len += printString(" at character ");
    len += printNum(loc);
    len += printString(":  ");
    printString(input_string.c_str());
    printString("\n");
    len += loc;
    // print error squiggles
    printRepeated(' ', len - 2);
    printString("~\n");
    flush();
}

void printError(string message, unsigned char beg, unsigned char end) {
    // print line 1
    int len = printString(message.c_str());
    len += printString(" at characters ");
    len += printNum(beg);
    len += printRepeated('-', 1u);
    len += printNum(end);
    len += printString(":  ");
    printString(input_string.c_str());
    printString("\n");
    len += beg;
    // print error squiggles
    printRepeated(' ', len - 3);
    printRepeated('~', (unsigned int) end - beg + 1);
    printRepeated('\n', 1);
    flush();
}