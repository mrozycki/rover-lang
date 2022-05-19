#pragma once

#include <memory>
#include <vector>

#include <token.h>

namespace rover {
class expression;
struct binary_op_expression;
struct unary_op_expression;
struct literal_expression;
struct identifier_expression;
struct function_call_expression;
struct array_literal_expression;
struct array_ref_expression;

class expression_visitor {
public:
    virtual ~expression_visitor() = default;
    virtual void visit(binary_op_expression const& node) = 0;
    virtual void visit(unary_op_expression const& node) = 0;
    virtual void visit(literal_expression const& node) = 0;
    virtual void visit(identifier_expression const& node) = 0;
    virtual void visit(function_call_expression const& node) = 0;
    virtual void visit(array_literal_expression const& node) = 0;
    virtual void visit(array_ref_expression const& node) = 0;
};

class expression {
public:
    virtual ~expression() = default;
    virtual void accept(expression_visitor& visitor) = 0;
};

struct binary_op_expression : public expression {
    binary_op_expression(std::unique_ptr<expression> left_, std::unique_ptr<expression> right_, token op_)
        : left(std::move(left_)), right(std::move(right_)), op(std::move(op_)) {}
    void accept(expression_visitor& visitor) override { visitor.visit(*this); }

    std::unique_ptr<expression> left;
    std::unique_ptr<expression> right;
    token op;
};

struct unary_op_expression : public expression {
    unary_op_expression(std::unique_ptr<expression> right_, token op_) : right(std::move(right_)), op(std::move(op_)) {}
    void accept(expression_visitor& visitor) override { visitor.visit(*this); }

    std::unique_ptr<expression> right;
    token op;
};

struct literal_expression : public expression {
    literal_expression(token value_) : literal(std::move(value_)) {}
    void accept(expression_visitor& visitor) override { visitor.visit(*this); }
    token literal;
};

struct identifier_expression : public expression {
    identifier_expression(token identifier_) : identifier(std::move(identifier_)) {}
    void accept(expression_visitor& visitor) override { visitor.visit(*this); }

    token identifier;
};

struct function_call_expression : public expression {
    function_call_expression(std::unique_ptr<expression> function_name_,
                             std::vector<std::unique_ptr<expression>> arguments_)
        : function_name(std::move(function_name_)), arguments(std::move(arguments_)) {}
    void accept(expression_visitor& visitor) override { visitor.visit(*this); }

    std::unique_ptr<expression> function_name;
    std::vector<std::unique_ptr<expression>> arguments;
};

struct array_literal_expression : public expression {
    array_literal_expression(std::vector<std::unique_ptr<expression>> elements_) : elements(std::move(elements_)) {}
    void accept(expression_visitor& visitor) override { visitor.visit(*this); }

    std::vector<std::unique_ptr<expression>> elements;
};

struct array_ref_expression : public expression {
    array_ref_expression(std::unique_ptr<expression> array_, std::unique_ptr<expression> index_)
        : array(std::move(array_)), index(std::move(index_)) {}
    void accept(expression_visitor& visitor) override { visitor.visit(*this); }

    std::unique_ptr<expression> array;
    std::unique_ptr<expression> index;
};

struct statement;
struct expression_statement;
struct block_statement;
struct definition_statement;
struct conditional_statement;
struct while_statement;

class statement_visitor {
public:
    virtual ~statement_visitor() = default;
    virtual void visit(expression_statement const& node) = 0;
    virtual void visit(block_statement const& node) = 0;
    virtual void visit(definition_statement const& node) = 0;
    virtual void visit(conditional_statement const& node) = 0;
    virtual void visit(while_statement const& node) = 0;
};

struct statement {
public:
    virtual ~statement() = default;
    virtual void accept(statement_visitor& visitor) = 0;
};

struct expression_statement : public statement {
    expression_statement(std::unique_ptr<expression> expr_) : expr(std::move(expr_)) {}
    void accept(statement_visitor& visitor) override { visitor.visit(*this); }

    std::unique_ptr<expression> expr;
};

struct block_statement : public statement {
    block_statement(std::vector<std::unique_ptr<statement>> statements_) : statements(std::move(statements_)) {}
    void accept(statement_visitor& visitor) override { visitor.visit(*this); }

    std::vector<std::unique_ptr<statement>> statements;
};

struct definition_statement : public statement {
    definition_statement(token identifier_, std::unique_ptr<expression> expr_, bool is_const_)
        : identifier(std::move(identifier_)), initializer(std::move(expr_)), is_const(is_const_) {}
    void accept(statement_visitor& visitor) override { visitor.visit(*this); }

    token identifier;
    std::unique_ptr<expression> initializer;
    bool is_const;
};

struct conditional_statement : public statement {
    conditional_statement(std::unique_ptr<expression> condition_, std::unique_ptr<statement> then_,
                          std::unique_ptr<statement> else_)
        : condition(std::move(condition_)), then_branch(std::move(then_)), else_branch(std::move(else_)) {}
    void accept(statement_visitor& visitor) override { visitor.visit(*this); }

    std::unique_ptr<expression> condition;
    std::unique_ptr<statement> then_branch;
    std::unique_ptr<statement> else_branch;
};

struct while_statement : public statement {
    while_statement(std::unique_ptr<expression> condition_, std::unique_ptr<statement> body_)
        : condition(std::move(condition_)), body(std::move(body_)) {}
    void accept(statement_visitor& visitor) override { visitor.visit(*this); }

    std::unique_ptr<expression> condition;
    std::unique_ptr<statement> body;
};
} // namespace rover
