#include "ast_printer.h"

namespace rover {
expression_printer::~expression_printer() {}

void expression_printer::visit(binary_op_expression const& node) {
    std::cout << "(";
    node.left->accept(*this);
    std::cout << " " << node.op << " ";
    node.right->accept(*this);
    std::cout << ")";
}

void expression_printer::visit(unary_op_expression const& node) {
    std::cout << node.op << "(";
    node.right->accept(*this);
    std::cout << ")";
}

void expression_printer::visit(literal_expression const& node) { std::cout << *node.literal.payload; }

void expression_printer::visit(identifier_expression const& node) { std::cout << *node.identifier.payload; }

void expression_printer::visit(function_call_expression const& node) {
    node.function_name->accept(*this);
    std::cout << "(";
    for (auto const& arg : node.arguments) {
        arg->accept(*this);
        std::cout << ", ";
    }
    std::cout << ")";
}

void expression_printer::visit(array_literal_expression const& node) {
    std::cout << "[";
    for (auto const& elem : node.elements) {
        elem->accept(*this);
        std::cout << ", ";
    }
    std::cout << "]";
}

void expression_printer::visit(array_ref_expression const& node) {
    std::cout << "[";
    node.array->accept(*this);
    std::cout << "]";
}

statement_printer::~statement_printer() {}

void statement_printer::visit(expression_statement const& node) {
    node.expr->accept(expr_printer);
    std::cout << ";";
}

void statement_printer::visit(block_statement const& node) {
    std::cout << "{\n";
    for (auto const& stmt : node.statements) {
        stmt->accept(*this);
        std::cout << "\n";
    }
    std::cout << "}";
}

void statement_printer::visit(definition_statement const& node) {
    if (node.is_const) {
        std::cout << "const ";
    } else {
        std::cout << "var ";
    }
    std::cout << *node.identifier.payload << " = ";
    node.initializer->accept(expr_printer);
    std::cout << ";";
}

void statement_printer::visit(conditional_statement const& node) {
    std::cout << "if (";
    node.condition->accept(expr_printer);
    std::cout << ") ";
    node.then_branch->accept(*this);
    if (node.else_branch) {
        std::cout << " else ";
        node.else_branch->accept(*this);
    }
}

void statement_printer::visit(while_statement const& node) {
    std::cout << "while (";
    node.condition->accept(expr_printer);
    std::cout << ") ";
    node.body->accept(*this);
}

} // namespace rover