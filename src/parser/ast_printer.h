#pragma once

#include "ast.h"

namespace rover {
class expression_printer : public expression_visitor {
public:
    virtual ~expression_printer();
    void visit(binary_op_expression const& node) override;
    void visit(unary_op_expression const& node) override;
    void visit(literal_expression const& node) override;
    void visit(identifier_expression const& node) override;
    void visit(function_call_expression const& node) override;
    void visit(array_literal_expression const& node) override;
    void visit(array_ref_expression const& node) override;
};

class statement_printer : public statement_visitor {
private:
    expression_printer expr_printer;

public:
    virtual ~statement_printer();
    void visit(expression_statement const& node) override;
    void visit(block_statement const& node) override;
    void visit(definition_statement const& node) override;
    void visit(conditional_statement const& node) override;
    void visit(while_statement const& node) override;
};
} // namespace rover