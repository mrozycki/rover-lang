#include <fstream>
#include <iostream>

#include "interpreter/context.h"
#include "interpreter/interpreter.h"
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
    rover::statement_executor executor(new rover::context(nullptr));
    for (auto& s : statements) {
        s->accept(executor);
    }

    return 0;
}