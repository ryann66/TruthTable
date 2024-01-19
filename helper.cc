#include <string>
#include <iostream>
#include <algorithm>

#include "helper.h"

#define INDENT "   "

using std::string;
using std::cout;
using std::endl;

inline void listPages() {
    cout << "The help pages are:" << endl;
    cout << INDENT "intro" << endl;
    cout << INDENT "modes" << endl;
    cout << INDENT "default" << endl;
    cout << INDENT "logical" << endl;
    cout << INDENT "boolean" << endl;
    cout << INDENT "cstyle" << endl;
    cout << INDENT "operators" << endl;
    cout << INDENT "variables" << endl;
    cout << INDENT "table" << endl;
}

inline void helpBasic(char* progName) {
    cout << progName << " is a tool for building truth tables" << endl;
    cout << "To use, enter a logical proposition like so:" << endl;
    cout << INDENT << progName << " \"a|b\"" << endl;
    cout << "Propositions are made of operators and variables.  Variables must be any alphabetical character" << endl;
    cout << "or, optionally, the constants 0 and 1 which will be evaluated as constants.  Operators vary by" << endl;
    cout << "mode, so be sure to check out the help page for each mode before use." << endl;
}

inline void helpModes() {
    cout << "Modes allow you to enter propositions in different styles" << endl;
    cout << "Different modes have different operators, to find out more about" << endl;
    cout << "a mode, use it's help page." << endl;
    cout << "The modes are: default logical boolean cstyle" << endl;
}

inline void helpDefault() {
    cout << "Default Mode -d" << endl;
    cout << "Recognized operators:" << endl;
    cout << "Not: ! ~ -" << endl;
    cout << "And: && & *" << endl;
    cout << "Or: || | +" << endl;
    cout << "Xor: ^" << endl;
    cout << "Implication: ->" << endl;
    cout << "Equivalent: = <->" << endl;
}

inline void helpLogical() {
    cout << "Logical Mode -l" << endl;
    cout << "Recognized operators:" << endl;
    cout << "Not: -" << endl;
    cout << "And: ^" << endl;
    cout << "Or: v" << endl;
    cout << "Xor: +" << endl;
    cout << "Implication: ->" << endl;
    cout << "Biconditional: <->" << endl;
}

inline void helpBoolean() {
    cout << "Boolean Mode -b" << endl;
    cout << "Recognized operators:" << endl;
    cout << "Not: -" << endl;
    cout << "And: *" << endl;
    cout << "Or: |" << endl;
    cout << "Xor: ^" << endl;
    cout << "Equivalent: =" << endl;
    cout << "Sequential variables will have And operators added (\"a&b\" == \"ab\")" << endl;
}

inline void helpCStyle() {
    cout << "CStyle Mode -c" << endl;
    cout << "Recognized operators:" << endl;
    cout << "Not: ! ~" << endl;
    cout << "And: && &" << endl;
    cout << "Or: || |" << endl;
    cout << "Xor: ^" << endl;
    cout << "Equivalent: =" << endl;
}

inline void helpOperators() {
    cout << "There are two types of operators: prefix and infix." << endl;
    cout << "Not is the only prefix operator, all others are infix" << endl;
    cout << "Operators precedence is handled as follows (highest -> lowest)" << endl;
    cout << "NOT AND XOR OR IMPLICATION EQUIVALENT" << endl;
    cout << "a && b || c = (a && b) || c" << endl;
}

inline void helpVariables() {
    cout << "Variables are values that can represent either 1 (T) or 0 (F)" << endl;
    cout << "Any uppercase or lowercase alphabetic character can be a variable." << endl;
    cout << "Additionally, '1' and '0' will be treated as constants that" << endl;
    cout << "always equal 1 (T) of 0 (F), respectively." << endl;
    cout << "If all propositions do not contain any variables, they will be" << endl;
    cout << "evaluated to a constant value (1 or 0), instead of building a table" << endl;
}

inline void helpTable() {
    cout << "The table can be printed to showcase differences" << endl;
    cout << "All Rows -a" << endl;
    cout << INDENT "Prints all rows in the table (default)" << endl;
    cout << "Emphasize Rows -e" << endl;
    cout << INDENT "Prints all rows, but highlights rows with differences" << endl;
    cout << "Select rows -s" << endl;
    cout << INDENT "Prints only rows with differences" << endl;
}

bool printHelp(int argc, char** argv, char* progName) {
    if (argc == 0) {
        cout << "Use " << progName << " [page] to access the help pages" << endl;
        listPages();
        return false;
    }
    bool foundInvalid = false;

    for (int i = 0; i < argc; i++) {
        if (i) {
            // add new line between pages
            cout << endl;
        }
        string arg(argv[i]);
        std::transform(arg.begin(), arg.end(), arg.begin(), [](unsigned char c){ return std::tolower(c); });
        if (arg == "default") {
            helpDefault();
            continue;
        }
        if (arg == "modes") {
            helpModes();
            continue;
        }
        if (arg == "logical") {
            helpLogical();
            continue;
        }
        if (arg == "boolean") {
            helpBoolean();
            continue;
        }
        if (arg == "cstyle") {
            helpCStyle();
            continue;
        }
        if (arg == "intro") {
            helpBasic(progName);
            continue;
        }
        if (arg == "operators") {
            helpOperators();
            continue;
        }
        if (arg == "variables") {
            helpVariables();
            continue;
        }
        // invalid help arg
        foundInvalid = true;
        cout << "Unknown help page: " << argv[i] << endl;
    }

    return foundInvalid;
}