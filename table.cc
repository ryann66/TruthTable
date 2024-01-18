#include <iostream>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <cctype>

#include "tokens.h"
#include "parser.h"
#include "interpreter.h"

#define VERSION "v1.0.1"
#define INDENT "   "

using std::queue;
using std::vector;
using std::set;
using std::string;
using std::cout;
using std::endl;

// todo advanced option selection
int main(int argc, char** argv) {
    if (argc < 2) {
        cout << "Usage: " << argv[0] << " [-option] propositions" << endl;
        return EXIT_FAILURE;
    }

    // Parse options and tokenize
    Style style = Default;
    set<char> variables;
    vector<queue<Token>> tokenLists;
    vector<const char*> inputStrings;
    for (int i = 1; i < argc; i++) {
        // look for an option flag followed by a single character
        if ((argv[i][0] == '-' || argv[i][0] == '/' || argv[i][0] == '\\') && argv[i][1] && !argv[i][2]) {
            switch (argv[i][1]) {
                case 'h':
                case '?':
                    // help
                    if (i + 1 != argc) {
                        string arg(argv[i + 1]);
                        std::transform(arg.begin(), arg.end(), arg.begin(), [](unsigned char c){ return std::tolower(c); });
                        if (arg == "version") {
                            cout << VERSION << endl;
                            i++;
                            continue;
                        }
                        if (arg == "default") {
                            // default help
                            cout << "Default Mode -d" << endl;
                            cout << "Recognized operators:" << endl;
                            cout << "Not: ! ~ -" << endl;
                            cout << "And: && & *" << endl;
                            cout << "Or: || | +" << endl;
                            cout << "Xor: ^" << endl;
                            cout << "Implication: ->" << endl;
                            cout << "Equivalent: = <->" << endl;
                            i++;
                            continue;
                        }
                        if (arg == "logical") {
                            // logical help
                            cout << "Logical Mode -l" << endl;
                            cout << "Recognized operators:" << endl;
                            cout << "Not: -" << endl;
                            cout << "And: ^" << endl;
                            cout << "Or: v" << endl;
                            cout << "Xor: +" << endl;
                            cout << "Implication: ->" << endl;
                            cout << "Biconditional: <->" << endl;
                            i++;
                            continue;
                        }
                        if (arg == "boolean") {
                            // boolean help
                            cout << "Boolean Mode -b" << endl;
                            cout << "Recognized operators:" << endl;
                            cout << "Not: -" << endl;
                            cout << "And: *" << endl;
                            cout << "Or: |" << endl;
                            cout << "Xor: ^" << endl;
                            cout << "Equivalent: =" << endl;
                            cout << "Sequential variables will have And operators added (\"a&b\" == \"ab\")" << endl;
                            i++;
                            continue;
                        }
                        if (arg == "cstyle") {
                            // cstyle help
                            cout << "CStyle Mode -c" << endl;
                            cout << "Recognized operators:" << endl;
                            cout << "Not: ! ~" << endl;
                            cout << "And: && &" << endl;
                            cout << "Or: || |" << endl;
                            cout << "Xor: ^" << endl;
                            cout << "Equivalent: =" << endl;
                            i++;
                            continue;
                        }
                        if (arg == "intro") {
                            // basic (intro) help
                            cout << argv[0] << " is a tool for building truth tables" << endl;
                            cout << "To use, enter a logical proposition like so:" << endl;
                            cout << INDENT << argv[0] << " \"a|b\"" << endl;
                            cout << "Propositions are made of operators and variables.  Variables must be any alphabetical character" << endl;
                            cout << "or, optionally, the constants 0 and 1 which will be evaluated as constants.  Operators vary by" << endl;
                            cout << "mode, so be sure to check out the help page for each mode before use." << endl;
                            i++;
                            continue;
                        }
                    }
                    // -h help
                    cout << "Use " << argv[i] << " [page] to access the help pages" << endl;
                    cout << "The help pages are:" << endl;
                    cout << INDENT "version" << endl;
                    cout << INDENT "intro" << endl;
                    cout << INDENT "default" << endl;
                    cout << INDENT "logical" << endl;
                    cout << INDENT "boolean" << endl;
                    cout << INDENT "cstyle" << endl;
                    continue;
                case 'l':
                    // logical
                    style = Logical;
                    continue;
                case 'b':
                    // boolean
                    style = Boolean;
                    continue;
                case 'c':
                    // cstyle
                    style = CStyle;
                    continue;
                case 'd':
                    // default
                    style = Default;
                    continue;
            }
        }
        // assume not option, tokenize
        tokenLists.push_back(tokenize(argv[i], variables, style));
        inputStrings.push_back(argv[i]);
    }
    // check tokenize success
    for (auto& tl : tokenLists) {
        if (tl.empty()) return EXIT_FAILURE;
    }

    // parse
    for (int i = 0; i < tokenLists.size(); i++) {
        tokenLists[i] = parseToRPN(tokenLists[i], inputStrings[i]);
    }
    // check parse success
    for (auto& tl : tokenLists) {
        if (tl.empty()) return EXIT_FAILURE;
    }

    // interpret
    if (interpretPropositions(tokenLists, variables)){
        return EXIT_SUCCESS;
    }
    return EXIT_FAILURE;
}