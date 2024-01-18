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
    cout << INDENT "default" << endl;
    cout << INDENT "logical" << endl;
    cout << INDENT "boolean" << endl;
    cout << INDENT "cstyle" << endl;
}

inline void helpBasic(char* progName) {
    cout << progName << " is a tool for building truth tables" << endl;
    cout << "To use, enter a logical proposition like so:" << endl;
    cout << INDENT << progName << " \"a|b\"" << endl;
    cout << "Propositions are made of operators and variables.  Variables must be any alphabetical character" << endl;
    cout << "or, optionally, the constants 0 and 1 which will be evaluated as constants.  Operators vary by" << endl;
    cout << "mode, so be sure to check out the help page for each mode before use." << endl;
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
        // invalid help arg
        foundInvalid = true;
        cout << "Unknown help page: " << argv[i] << endl;
    }

    return foundInvalid;
}