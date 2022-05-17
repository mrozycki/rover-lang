#pragma once

#include <memory>
#include <vector>

#include <lexer.h>

#include "ast.h"

namespace rover {
class parser {
private:
    lexer lexer_;

    std::unique_ptr<rover::expression> expression();
    std::unique_ptr<rover::expression> assignment();
    std::unique_ptr<rover::expression> logic_or();
    std::unique_ptr<rover::expression> logic_and();
    std::unique_ptr<rover::expression> equality();
    std::unique_ptr<rover::expression> comparison();
    std::unique_ptr<rover::expression> term();
    std::unique_ptr<rover::expression> factor();
    std::unique_ptr<rover::expression> unary();
    std::unique_ptr<rover::expression> postfix();
    std::unique_ptr<rover::expression> primary();

    std::unique_ptr<rover::statement> statement();
    std::unique_ptr<rover::statement> expression_statement();
    std::unique_ptr<rover::statement> if_statement();
    std::unique_ptr<rover::statement> while_statement();
    std::unique_ptr<rover::statement> block_statement();
    std::unique_ptr<rover::statement> definition_statement();

public:
    parser(lexer l);
    std::vector<std::unique_ptr<rover::statement>> parse();
};
} // namespace rover