#pragma once

#include <iostream>
#include <variant>

#include <ast.h>

#include "context.h"
#include "value.h"

namespace rover {
class expression_evaluator : public expression_visitor {
private:
    context* ctx;
    value* get(expression const& node);

    void report_error(std::string const& msg) { std::cout << "Interpreter error: " << msg << "\n"; }

public:
    explicit expression_evaluator(context* ctx_);
    virtual ~expression_evaluator();
    void visit(binary_op_expression const& node) override;
    void visit(unary_op_expression const& node) override;
    void visit(literal_expression const& node) override;
    void visit(identifier_expression const& node) override;
    void visit(function_call_expression const& node) override;
    void visit(array_literal_expression const& node) override;
    void visit(array_ref_expression const& node) override;

    value result;
};

class statement_executor : public statement_visitor {
private:
    context* ctx;

    void report_error(std::string const& msg) { std::cout << "Interpreter error: " << msg << "\n"; }

public:
    explicit statement_executor(context* ctx_);
    virtual ~statement_executor();
    void visit(expression_statement const& node) override;
    void visit(block_statement const& node) override;
    void visit(definition_statement const& node) override;
    void visit(conditional_statement const& node) override;
    void visit(while_statement const& node) override;
};
} // namespace rover