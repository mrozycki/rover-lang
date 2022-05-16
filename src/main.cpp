#include <fstream>
#include <iostream>

#include "lexer/lexer.h"
#include "lexer/token.h"

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <file>" << std::endl;
        return 1;
    }

    std::ifstream input(argv[1]);
    if (!input) {
        std::cerr << "Could not open file: " << argv[1] << std::endl;
        return 1;
    }

    rover::lexer lexer(input);
    
    while (auto const &token = lexer.peek()) {
        std::cout << *token << "\n";
        lexer.consume();
    }

    return 0;
}