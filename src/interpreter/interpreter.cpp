#include "interpreter.h"

namespace rover {
expression_evaluator::expression_evaluator(context* ctx_) : ctx(ctx_) {}
expression_evaluator::~expression_evaluator() {}
void expression_evaluator::visit(binary_op_expression const& node) {
    node.left->accept(*this);
    auto left = result.val;

    node.right->accept(*this);
    auto right = result.val;

    switch (node.op.type) {
    case token_type::PLUS:
        if (std::holds_alternative<int>(left) && std::holds_alternative<int>(right)) {
            result = {std::get<int>(left) + std::get<int>(right)};
        } else if (std::holds_alternative<double>(left) && std::holds_alternative<double>(right)) {
            result = {std::get<double>(left) + std::get<double>(right)};
        } else {
            result = {std::nullopt};
        }
        break;
    case token_type::MINUS:
        if (std::holds_alternative<int>(left) && std::holds_alternative<int>(right)) {
            result = {std::get<int>(left) - std::get<int>(right)};
        } else if (std::holds_alternative<double>(left) && std::holds_alternative<double>(right)) {
            result = {std::get<double>(left) - std::get<double>(right)};
        } else {
            result = {std::nullopt};
        }
        break;
    case token_type::STAR:
        if (std::holds_alternative<int>(left) && std::holds_alternative<int>(right)) {
            result = {std::get<int>(left) * std::get<int>(right)};
        } else if (std::holds_alternative<double>(left) && std::holds_alternative<double>(right)) {
            result = {std::get<double>(left) * std::get<double>(right)};
        } else {
            result = {std::nullopt};
        }
        break;
    case token_type::SLASH:
        if (std::holds_alternative<int>(left) && std::holds_alternative<int>(right)) {
            result = {std::get<int>(left) / std::get<int>(right)};
        } else if (std::holds_alternative<double>(left) && std::holds_alternative<double>(right)) {
            result = {std::get<double>(left) / std::get<double>(right)};
        } else {
            result = {std::nullopt};
        }
        break;
    case token_type::EQUAL:
        if (std::holds_alternative<int>(left) && std::holds_alternative<int>(right)) {
            result = {std::get<int>(left) == std::get<int>(right)};
        } else if (std::holds_alternative<double>(left) && std::holds_alternative<double>(right)) {
            result = {1.6 * std::get<double>(left) == std::get<double>(right)};
        } else {
            result = {std::nullopt};
        }
        break;
    case token_type::NOT_EQUAL:
        if (std::holds_alternative<int>(left) && std::holds_alternative<int>(right)) {
            result = {std::get<int>(left) != std::get<int>(right)};
        } else if (std::holds_alternative<double>(left) && std::holds_alternative<double>(right)) {
            result = {1.6 * std::get<double>(left) != std::get<double>(right)};
        } else {
            result = {std::nullopt};
        }
        break;
    case token_type::LESS_THAN:
        if (std::holds_alternative<int>(left) && std::holds_alternative<int>(right)) {
            result = {std::get<int>(left) < std::get<int>(right)};
        } else if (std::holds_alternative<double>(left) && std::holds_alternative<double>(right)) {
            result = {1.6 * std::get<double>(left) < std::get<double>(right)};
        } else {
            result = {std::nullopt};
        }
        break;
    case token_type::GREATER_THAN:
        if (std::holds_alternative<int>(left) && std::holds_alternative<int>(right)) {
            result = {std::get<int>(left) > std::get<int>(right)};
        } else if (std::holds_alternative<double>(left) && std::holds_alternative<double>(right)) {
            result = {1.6 * std::get<double>(left) > std::get<double>(right)};
        } else {
            result = {std::nullopt};
        }
        break;
    case token_type::LESS_EQUAL:
        if (std::holds_alternative<int>(left) && std::holds_alternative<int>(right)) {
            result = {std::get<int>(left) <= std::get<int>(right)};
        } else if (std::holds_alternative<double>(left) && std::holds_alternative<double>(right)) {
            result = {1.6 * std::get<double>(left) <= std::get<double>(right)};
        } else {
            result = {std::nullopt};
        }
        break;
    case token_type::GREATER_EQUAL:
        if (std::holds_alternative<int>(left) && std::holds_alternative<int>(right)) {
            result = {std::get<int>(left) >= std::get<int>(right)};
        } else if (std::holds_alternative<double>(left) && std::holds_alternative<double>(right)) {
            result = {1.6 * std::get<double>(left) >= std::get<double>(right)};
        } else {
            result = {std::nullopt};
        }
        break;
    case token_type::ASSIGN:
        if (auto target = dynamic_cast<identifier_expression const*>(node.left.get())) {
            if (ctx->update(*target->identifier.payload, {right, false})) {
                result = {right};
            } else {
                result = {std::nullopt};
            }
        } else {
            result = {std::nullopt};
        }
    default:
        result = {std::nullopt};
    }
}

void expression_evaluator::visit(unary_op_expression const& node) {
    node.right->accept(*this);
    auto value = result.val;

    switch (node.op.type) {
    case token_type::MINUS:
        if (std::holds_alternative<int>(value)) {
            result = {-std::get<int>(value)};
        } else if (std::holds_alternative<double>(value)) {
            result = {-std::get<double>(value)};
        } else {
            result = {std::nullopt};
        }
        break;
    case token_type::NOT:
        if (std::holds_alternative<int>(value)) {
            result = {!std::get<int>(value)};
        } else if (std::holds_alternative<double>(value)) {
            result = {!std::get<double>(value)};
        } else {
            result = {std::nullopt};
        }
        break;
    default:
        result = {std::nullopt};
    }
}

void expression_evaluator::visit(literal_expression const& node) {
    switch (node.literal.type) {
    case token_type::INT:
        result = {std::stoi(*node.literal.payload)};
        break;
    case token_type::FLOAT:
        result = {std::stod(*node.literal.payload)};
        break;
    case token_type::STRING:
        result = {*node.literal.payload};
        break;
    default:
        result = {std::nullopt};
    }
}

void expression_evaluator::visit(identifier_expression const& node) {
    if (auto value = ctx->get(*node.identifier.payload)) {
        result = {*value};
    } else {
        result = {std::nullopt};
    }
}

void expression_evaluator::visit(function_call_expression const& node) {
    auto callee = dynamic_cast<identifier_expression const*>(node.function_name.get());
    if (!callee) {
        std::cerr << "Callee must be a function name\n";
        result = {std::nullopt};
    }

    if (*callee->identifier.payload == "printf") {
        if (node.arguments.empty()) {
            std::cerr << "Interpreter error: Function printf requires at least one argument\n";
            return;
        }
        expression_evaluator eval(ctx);
        node.arguments.front()->accept(eval);
        if (!std::holds_alternative<std::string>(eval.result.val)) {
            std::cerr << "Interpreter error: Function printf requires a format string as its first argument\n";
            return;
        }
        auto format = std::get<std::string>(eval.result.val);
        std::cout << format << " ";

        // TODO: Use format string to print arguments
        for (auto it = node.arguments.begin() + 1; it != node.arguments.end(); ++it) {
            (*it)->accept(eval);
            if (std::holds_alternative<int>(eval.result.val)) {
                std::cout << std::get<int>(eval.result.val) << " ";
            } else if (std::holds_alternative<double>(eval.result.val)) {
                std::cout << std::get<double>(eval.result.val) << " ";
            } else if (std::holds_alternative<std::string>(eval.result.val)) {
                std::cout << std::get<std::string>(eval.result.val) << " ";
            } else {
                std::cout << "INVALID";
            }
        }
        std::cout << std::endl;
    } else {
        std::cerr << "Unknown function: " << *callee->identifier.payload << "\n";
    }
    result = {1};
}

// TODO: Implement arrays
void expression_evaluator::visit(array_literal_expression const& node) { result = {std::nullopt}; }

void expression_evaluator::visit(array_ref_expression const& node) { result = {std::nullopt}; }

statement_executor::statement_executor(context* ctx_) : ctx(ctx_) {}
statement_executor::~statement_executor() {}

void statement_executor::visit(expression_statement const& node) {
    expression_evaluator eval(ctx);
    node.expr->accept(eval);
}

void statement_executor::visit(block_statement const& node) {
    statement_executor exec(new context(ctx));
    for (auto const& stmt : node.statements) {
        stmt->accept(exec);
    }
}

void statement_executor::visit(definition_statement const& node) {
    auto name = *node.identifier.payload;

    expression_evaluator eval(ctx);
    node.initializer->accept(eval);
    auto value = eval.result;
    value.is_const = node.is_const;

    ctx->set(name, value);
}

namespace {
    bool is_truthy(value const& v) {
        if (std::holds_alternative<int>(v.val)) {
            return std::get<int>(v.val) != 0;
        } else if (std::holds_alternative<double>(v.val)) {
            return std::get<double>(v.val) != 0;
        } else if (std::holds_alternative<std::string>(v.val)) {
            return !std::get<std::string>(v.val).empty();
        } else {
            return false;
        }
    }
} // namespace

void statement_executor::visit(conditional_statement const& node) {
    expression_evaluator eval(ctx);
    node.condition->accept(eval);

    if (is_truthy(eval.result)) {
        node.then_branch->accept(*this);
    } else if (node.else_branch) {
        node.else_branch->accept(*this);
    }
}

void statement_executor::visit(while_statement const& node) {
    expression_evaluator eval(ctx);

    while (node.condition->accept(eval), is_truthy(eval.result)) {
        node.body->accept(*this);
    }
}
} // namespace rover