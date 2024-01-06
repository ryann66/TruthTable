#include <iostream>
#include <cstdlib>

#include "tokens.h"

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

    
    while (!tokenlist.empty()) {
        cout << tokenlist.front();
        tokenlist.pop();
    }
    cout << endl;
    return EXIT_SUCCESS;
}