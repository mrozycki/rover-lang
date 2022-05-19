#pragma once

#include <memory>
#include <vector>

#include <lexer.h>

#include "ast.h"

namespace rover {

class parser {
private:
    lexer lexer_;

    std::vector<std::string> errors_;

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

    void report_error(std::string const& message, std::optional<token> const& t) {
        if (t) {
            report_error(message, t->line, t->column);
        } else {
            report_error(message);
        }
    }

    void report_error(std::string const& message, std::size_t line, std::size_t column) {
        errors_.push_back(message + " in line " + std::to_string(line) + ", column " + std::to_string(column));
    }

    void report_error(std::string const& message) { errors_.push_back(message + "at end of file"); }

public:
    parser(lexer l);
    std::vector<std::unique_ptr<rover::statement>> parse();
    std::vector<std::string> errors() const { return errors_; }
};
} // namespace rover