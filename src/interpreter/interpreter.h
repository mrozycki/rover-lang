#pragma once

#include <variant>

#include <ast.h>

namespace rover {
using value = std::variant<int, double, std::string, std::nullopt_t>;

class expression_evaluator : public expression_visitor {
public:
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
public:
    virtual ~statement_executor();
    void visit(expression_statement const& node) override;
    void visit(block_statement const& node) override;
    void visit(definition_statement const& node) override;
    void visit(conditional_statement const& node) override;
    void visit(while_statement const& node) override;
};
} // namespace rover