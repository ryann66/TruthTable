#include <iostream>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <cctype>

#include "tokens.h"
#include "parser.h"
#include "interpreter.h"
#include "helper.h"

#define VERSION "v1.0.2"

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

    // check for help parse (must be arg 1)
    if (argv[1][0] == '-' || argv[1][0] == '/' || argv[1][0] == '\\') {
        if ((argv[1][1] == '?' || argv[1][1] == 'h') && !argv[1][2]) {
            // print help
            return printHelp(argc - 2, argv + 2, argv[0]) ? EXIT_FAILURE : EXIT_SUCCESS;
        }
        if (argv[1][1] == 'v' && !argv[1][2]) {
            // print version
            cout << VERSION << endl;
        }
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