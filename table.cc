#include <iostream>
#include <cstdlib>

#include "tokens.h"
#include "parser.h"

using std::queue;
using std::set;
using std::cout;
using std::cerr;
using std::endl;

int main(int argc, char** argv) {
    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " <logical statement>" << endl;
        return EXIT_FAILURE;
    }
    set<char> variables;
    queue<Token> tokenlist = tokenize(argv[1], variables);
    if (tokenlist.empty()) {
        return EXIT_FAILURE;
    }

    queue<Token> reversepolish = parseToRPN(tokenlist);
    if (reversepolish.empty()) {
        return EXIT_FAILURE;
    }

    while (!reversepolish.empty()) {
        cout << reversepolish.front();
        reversepolish.pop();
    }
    cout << endl;
    return EXIT_SUCCESS;
}