#include "parser.h"

#include <token.h>

#include "ast.h"

namespace rover {
parser::parser(lexer l) : lexer_(l) {}

std::unique_ptr<expression> parser::expression() { return assignment(); }

std::unique_ptr<expression> parser::assignment() {
    auto e = logic_or();
    if (!e) {
        return {};
    }

    auto t = lexer_.consume_if({token_type::ASSIGN});
    if (!t) {
        return e;
    }

    auto e2 = expression();
    if (!e2) {
        return {};
    }

    return std::make_unique<binary_op_expression>(std::move(e), std::move(e2), *t);
}

std::unique_ptr<expression> parser::logic_or() {
    auto left = logic_and();
    if (!left) {
        return {};
    }

    while (auto t = lexer_.consume_if({token_type::OR})) {
        auto right = logic_and();
        if (!right) {
            return {};
        }

        left = std::make_unique<binary_op_expression>(std::move(left), std::move(right), *t);
    }

    return left;
}

std::unique_ptr<expression> parser::logic_and() {
    auto e = equality();
    if (!e) {
        return {};
    }

    auto t = lexer_.consume_if({token_type::AND});
    if (!t) {
        return e;
    }

    auto e2 = expression();
    if (!e2) {
        return {};
    }

    return std::make_unique<binary_op_expression>(std::move(e), std::move(e2), *t);
}

std::unique_ptr<expression> parser::equality() {
    auto left = comparison();
    if (!left) {
        return {};
    }

    while (auto t = lexer_.consume_if({token_type::EQUAL, token_type::NOT_EQUAL})) {
        auto right = comparison();
        if (!right) {
            return {};
        }

        left = std::make_unique<binary_op_expression>(std::move(left), std::move(right), *t);
    }

    return left;
}

std::unique_ptr<expression> parser::comparison() {
    auto left = term();
    if (!left) {
        return {};
    }

    while (auto t = lexer_.consume_if(
               {token_type::LESS_THAN, token_type::LESS_EQUAL, token_type::GREATER_THAN, token_type::GREATER_EQUAL})) {
        auto right = term();
        if (!right) {
            return {};
        }

        left = std::make_unique<binary_op_expression>(std::move(left), std::move(right), *t);
    }

    return left;
}

std::unique_ptr<expression> parser::term() {
    auto left = factor();
    if (!left) {
        return {};
    }

    while (auto t = lexer_.consume_if({token_type::STAR, token_type::SLASH})) {
        auto right = factor();
        if (!right) {
            return {};
        }

        left = std::make_unique<binary_op_expression>(std::move(left), std::move(right), *t);
    }

    return left;
}

std::unique_ptr<expression> parser::factor() {
    auto left = unary();
    if (!left) {
        return {};
    }

    while (auto t = lexer_.consume_if({token_type::PLUS, token_type::MINUS})) {
        auto right = unary();
        if (!right) {
            return {};
        }

        left = std::make_unique<binary_op_expression>(std::move(left), std::move(right), *t);
    }

    return left;
}

std::unique_ptr<expression> parser::unary() {
    auto t = lexer_.consume_if({token_type::NOT, token_type::MINUS});
    if (!t) {
        return postfix();
    }

    auto e = postfix();
    if (!e) {
        return {};
    }

    return std::make_unique<unary_op_expression>(std::move(e), *t);
}

std::unique_ptr<expression> parser::postfix() {
    auto left = primary();
    if (!left) {
        return {};
    }

    while (auto t = lexer_.consume_if({token_type::LEFT_PAREN, token_type::LEFT_SQUARE})) {
        if (t->type == token_type::LEFT_PAREN) {
            std::vector<std::unique_ptr<rover::expression>> args;
            do {
                auto arg = expression();
                if (!arg) {
                    return {};
                }

                args.push_back(std::move(arg));
            } while (lexer_.consume_if({token_type::COMMA}));

            if (!lexer_.consume_if({token_type::RIGHT_PAREN})) {
                return {};
            }

            left = std::make_unique<function_call_expression>(std::move(left), std::move(args));
        } else if (t->type == token_type::LEFT_SQUARE) {
            auto index = expression();
            if (!index) {
                return {};
            }

            if (!lexer_.consume_if({token_type::RIGHT_SQUARE})) {
                return {};
            }

            left = std::make_unique<array_ref_expression>(std::move(left), std::move(index));
        }
    }

    return left;
}
std::unique_ptr<expression> parser::primary() {
    auto t = lexer_.consume();
    if (!t) {
        return {};
    }

    switch (t->type) {
    case token_type::IDENTIFIER:
        return std::make_unique<identifier_expression>(*t);
    case token_type::INT:
    case token_type::FLOAT:
    case token_type::STRING:
        return std::make_unique<literal_expression>(*t);
    case token_type::LEFT_PAREN: {
        auto e = expression();
        if (!e) {
            return {};
        }

        if (!lexer_.consume_if({token_type::RIGHT_PAREN})) {
            return {};
        }

        return e;
    }
    case token_type::LEFT_SQUARE: {
        std::vector<std::unique_ptr<rover::expression>> elements;

        do {
            auto elem = expression();
            if (!elem) {
                return {};
            }

            elements.push_back(std::move(elem));
        } while (lexer_.consume_if({token_type::COMMA}));

        if (!lexer_.consume_if({token_type::RIGHT_SQUARE})) {
            return {};
        }

        return std::make_unique<array_literal_expression>(std::move(elements));
    }
    default:
        return {};
    }
}

std::vector<std::unique_ptr<rover::statement>> parser::parse() {
    std::vector<std::unique_ptr<rover::statement>> statements;

    while (lexer_.peek()->type != token_type::END_OF_FILE) {
        if (auto s = statement()) {
            statements.push_back(std::move(s));
        } else {
            return {};
        }
    }

    return statements;
}

std::unique_ptr<rover::statement> parser::statement() {
    auto t = lexer_.peek();
    if (!t) {
        return {};
    }

    switch (t->type) {
    case token_type::IF:
        return if_statement();
    case token_type::WHILE:
        return while_statement();
    case token_type::LEFT_BRACE:
        return block_statement();
    case token_type::CONST:
    case token_type::VAR:
        return definition_statement();
    default:
        return expression_statement();
    }
}

std::unique_ptr<rover::statement> parser::expression_statement() {
    auto expr = expression();
    if (!expr) {
        return {};
    }

    if (!lexer_.consume_if({token_type::SEMICOLON})) {
        return {};
    }

    return std::make_unique<rover::expression_statement>(std::move(expr));
}
std::unique_ptr<rover::statement> parser::if_statement() {
    if (!lexer_.consume_if({token_type::IF})) {
        return {};
    }

    auto condition = expression();
    if (!condition) {
        return {};
    }

    auto then_branch = block_statement();
    if (!then_branch) {
        return {};
    }

    if (lexer_.peek()->type == token_type::ELSE) {
        lexer_.consume();

        if (lexer_.peek()->type == token_type::IF) {
            auto else_branch = if_statement();
            if (!else_branch) {
                return {};
            }

            return std::make_unique<conditional_statement>(std::move(condition), std::move(then_branch),
                                                           std::move(else_branch));
        } else if (lexer_.peek()->type == token_type::LEFT_BRACE) {
            auto else_branch = block_statement();
            if (!else_branch) {
                return {};
            }

            return std::make_unique<conditional_statement>(std::move(condition), std::move(then_branch),
                                                           std::move(else_branch));
        } else {
            return {};
        }
    } else {
        return std::make_unique<conditional_statement>(std::move(condition), std::move(then_branch), nullptr);
    }
}

std::unique_ptr<rover::statement> parser::while_statement() {
    if (!lexer_.consume_if({token_type::WHILE})) {
        return {};
    }

    auto condition = expression();
    if (!condition) {
        return {};
    }

    auto body = block_statement();
    if (!body) {
        return {};
    }

    return std::make_unique<rover::while_statement>(std::move(condition), std::move(body));
}

std::unique_ptr<rover::statement> parser::block_statement() {
    if (!lexer_.consume_if({token_type::LEFT_BRACE})) {
        return {};
    }

    std::vector<std::unique_ptr<rover::statement>> statements;

    while (lexer_.peek()->type != token_type::RIGHT_BRACE) {
        if (auto s = statement()) {
            statements.push_back(std::move(s));
        } else {
            return {};
        }
    }

    if (!lexer_.consume_if({token_type::RIGHT_BRACE})) {
        return {};
    }

    return std::make_unique<rover::block_statement>(std::move(statements));
}

std::unique_ptr<rover::statement> parser::definition_statement() {
    auto t = lexer_.consume_if({token_type::CONST, token_type::VAR});
    if (!t) {
        return {};
    }

    auto name = lexer_.consume_if({token_type::IDENTIFIER});
    if (!name) {
        return {};
    }

    if (!lexer_.consume_if({token_type::ASSIGN})) {
        return {};
    }

    auto value = expression();
    if (!value) {
        return {};
    }

    if (!lexer_.consume_if({token_type::SEMICOLON})) {
        return {};
    }

    return std::make_unique<rover::definition_statement>(*name, std::move(value), t->type == token_type::CONST);
}
} // namespace rover
