#include <fstream>
#include <iostream>

#include "lexer/lexer.h"
#include "lexer/token.h"
#include "parser/ast_printer.h"
#include "parser/parser.h"

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
    rover::parser parser(std::move(lexer));

    auto statements = parser.parse();
    rover::statement_printer printer;
    for (auto& s : statements) {
        s->accept(printer);
        std::cout << "\n";
    }
    std::cout << "Parsed " << statements.size() << " statements\n";

    return 0;
}