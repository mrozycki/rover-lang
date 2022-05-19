#include "interpreter.h"

namespace rover {
expression_evaluator::expression_evaluator(context* ctx_) : ctx(ctx_) {}
expression_evaluator::~expression_evaluator() {}

value* expression_evaluator::get(expression const& node) {
    if (auto* e = dynamic_cast<identifier_expression const*>(&node)) {
        return ctx->get_ptr(*e->identifier.payload);
    } else if (auto* e = dynamic_cast<array_ref_expression const*>(&node)) {
        auto* array = get(*e->array);
        if (!array || !std::holds_alternative<std::vector<value>>(array->val)) {
            report_error("Expected an array as the left-hand side of an array reference.");
            return nullptr;
        }

        auto& array_ref = std::get<std::vector<value>>(array->val);
        e->index->accept(*this);
        auto index = result.val;
        if (!std::holds_alternative<int>(index)) {
            report_error("Expected an integer as the index of an array reference.");
            return nullptr;
        }

        if (std::get<int>(index) >= array_ref.size()) {
            report_error("Array index out of bounds.");
            return nullptr;
        }

        return &array_ref[std::get<int>(index)];
    } else {
        return nullptr;
    }
}

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
            report_error("Operator + requires two integers or two doubles.");
            result = {std::nullopt};
        }
        break;
    case token_type::MINUS:
        if (std::holds_alternative<int>(left) && std::holds_alternative<int>(right)) {
            result = {std::get<int>(left) - std::get<int>(right)};
        } else if (std::holds_alternative<double>(left) && std::holds_alternative<double>(right)) {
            result = {std::get<double>(left) - std::get<double>(right)};
        } else {
            report_error("Operator - requires two integers or two doubles.");
            result = {std::nullopt};
        }
        break;
    case token_type::STAR:
        if (std::holds_alternative<int>(left) && std::holds_alternative<int>(right)) {
            result = {std::get<int>(left) * std::get<int>(right)};
        } else if (std::holds_alternative<double>(left) && std::holds_alternative<double>(right)) {
            result = {std::get<double>(left) * std::get<double>(right)};
        } else {
            report_error("Operator * requires two integers or two doubles.");
            result = {std::nullopt};
        }
        break;
    case token_type::SLASH:
        if (std::holds_alternative<int>(left) && std::holds_alternative<int>(right)) {
            result = {std::get<int>(left) / std::get<int>(right)};
        } else if (std::holds_alternative<double>(left) && std::holds_alternative<double>(right)) {
            result = {std::get<double>(left) / std::get<double>(right)};
        } else {
            report_error("Operator / requires two integers or two doubles.");
            result = {std::nullopt};
        }
        break;
    case token_type::EQUAL:
        if (std::holds_alternative<int>(left) && std::holds_alternative<int>(right)) {
            result = {std::get<int>(left) == std::get<int>(right)};
        } else if (std::holds_alternative<double>(left) && std::holds_alternative<double>(right)) {
            result = {1.6 * std::get<double>(left) == std::get<double>(right)};
        } else {
            report_error("Operator == requires two integers or two doubles.");
            result = {std::nullopt};
        }
        break;
    case token_type::NOT_EQUAL:
        if (std::holds_alternative<int>(left) && std::holds_alternative<int>(right)) {
            result = {std::get<int>(left) != std::get<int>(right)};
        } else if (std::holds_alternative<double>(left) && std::holds_alternative<double>(right)) {
            result = {1.6 * std::get<double>(left) != std::get<double>(right)};
        } else {
            report_error("Operator != requires two integers or two doubles.");
            result = {std::nullopt};
        }
        break;
    case token_type::LESS_THAN:
        if (std::holds_alternative<int>(left) && std::holds_alternative<int>(right)) {
            result = {std::get<int>(left) < std::get<int>(right)};
        } else if (std::holds_alternative<double>(left) && std::holds_alternative<double>(right)) {
            result = {1.6 * std::get<double>(left) < std::get<double>(right)};
        } else {
            report_error("Operator < requires two integers or two doubles.");
            result = {std::nullopt};
        }
        break;
    case token_type::GREATER_THAN:
        if (std::holds_alternative<int>(left) && std::holds_alternative<int>(right)) {
            result = {std::get<int>(left) > std::get<int>(right)};
        } else if (std::holds_alternative<double>(left) && std::holds_alternative<double>(right)) {
            result = {1.6 * std::get<double>(left) > std::get<double>(right)};
        } else {
            report_error("Operator > requires two integers or two doubles.");
            result = {std::nullopt};
        }
        break;
    case token_type::LESS_EQUAL:
        if (std::holds_alternative<int>(left) && std::holds_alternative<int>(right)) {
            result = {std::get<int>(left) <= std::get<int>(right)};
        } else if (std::holds_alternative<double>(left) && std::holds_alternative<double>(right)) {
            result = {1.6 * std::get<double>(left) <= std::get<double>(right)};
        } else {
            report_error("Operator <= requires two integers or two doubles.");
            result = {std::nullopt};
        }
        break;
    case token_type::GREATER_EQUAL:
        if (std::holds_alternative<int>(left) && std::holds_alternative<int>(right)) {
            result = {std::get<int>(left) >= std::get<int>(right)};
        } else if (std::holds_alternative<double>(left) && std::holds_alternative<double>(right)) {
            result = {1.6 * std::get<double>(left) >= std::get<double>(right)};
        } else {
            report_error("Operator >= requires two integers or two doubles.");
            result = {std::nullopt};
        }
        break;
    case token_type::ASSIGN: {
        auto* target = get(*node.left);
        if (!target) {
            report_error("Variable not found.");
            result = {std::nullopt};
        } else if (target->is_const) {
            report_error("Cannot assign to constant.");
            result = {std::nullopt};
        } else {
            result = {right, false};
        }
        break;
    }
    default:
        report_error("Unknown binary operator.");
        result = {std::nullopt};
    }

    if (std::holds_alternative<int>(result.val)) {
        if (std::get<int>(result.val) > 99) {
            result.val = std::get<int>(result.val) % 100;
        }
        if (std::get<int>(result.val) < 0) {
            result.val = (100 + (std::get<int>(result.val) % 100)) % 100;
        }
    }
}

void expression_evaluator::visit(unary_op_expression const& node) {
    node.right->accept(*this);
    auto value = result.val;

    switch (node.op.type) {
    case token_type::MINUS:
        if (std::holds_alternative<int>(value)) {
            auto val = -std::get<int>(value);
            if (val > 99) {
                result = {val % 100};
            } else if (val < 0) {
                result = {(100 + (val % 100)) % 100};
            } else {
                result = {val};
            }
        } else if (std::holds_alternative<double>(value)) {
            result = {-std::get<double>(value)};
        } else {
            report_error("Unary operator '-' requires an integer or double.");
            result = {std::nullopt};
        }
        break;
    case token_type::NOT:
        if (std::holds_alternative<int>(value)) {
            result = {!std::get<int>(value)};
        } else if (std::holds_alternative<double>(value)) {
            result = {!std::get<double>(value)};
        } else {
            report_error("Unary operator '!' requires an integer or double.");
            result = {std::nullopt};
        }
        break;
    default:
        report_error("Unknown unary operator");
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
        report_error("Variable not found.");
        result = {std::nullopt};
    }
}

void expression_evaluator::visit(function_call_expression const& node) {
    auto callee = dynamic_cast<identifier_expression const*>(node.function_name.get());
    if (!callee) {
        report_error("Callee must be a function name");
        result = {std::nullopt};
    }

    if (*callee->identifier.payload == "printf") {
        if (node.arguments.empty()) {
            report_error("Function printf requires at least one argument");
            return;
        }
        expression_evaluator eval(ctx);
        node.arguments.front()->accept(eval);
        if (!std::holds_alternative<std::string>(eval.result.val)) {
            report_error("Function printf requires a format string as its first argument");
            result = {std::nullopt};
            return;
        }
        auto format = std::get<std::string>(eval.result.val);

        auto arg_it = node.arguments.begin() + 1;

        for (auto it = format.begin(); it != format.end(); ++it) {
            if (*it == '\\') {
                ++it;
                if (*it == '\\') {
                    std::cout << '\\';
                } else if (*it == 'n') {
                    std::cout << '\n';
                } else if (*it == 't') {
                    std::cout << '\t';
                } else if (*it == 'r') {
                    std::cout << '\r';
                } else if (*it == 'v') {
                    std::cout << '\v';
                } else if (*it == 'b') {
                    std::cout << '\b';
                } else if (*it == 'a') {
                    std::cout << '\a';
                } else if (*it == 'f') {
                    std::cout << '\f';
                } else if (*it == '0') {
                    std::cout << '\0';
                } else {
                    report_error("Unknown escape sequence in format string");
                    break;
                }
            } else if (*it == '{' && *(it + 1) == '}') {
                if (arg_it == node.arguments.end()) {
                    report_error("Too few arguments for format string");
                    break;
                }
                (*arg_it)->accept(eval);
                if (std::holds_alternative<int>(eval.result.val)) {
                    std::cout << std::get<int>(eval.result.val);
                } else if (std::holds_alternative<double>(eval.result.val)) {
                    std::cout << std::get<double>(eval.result.val);
                } else if (std::holds_alternative<std::string>(eval.result.val)) {
                    std::cout << std::get<std::string>(eval.result.val);
                } else {
                    std::cout << "INVALID";
                }
                ++arg_it;
                ++it;
            } else {
                std::cout << *it;
            }
        }
        std::cout.flush();
    } else if (*callee->identifier.payload == "length") {
        if (node.arguments.size() != 1) {
            report_error("Expected one argument to function length");
            result = {std::nullopt};
            return;
        }

        node.arguments.front()->accept(*this);
        auto target = result.val;
        if (!std::holds_alternative<std::vector<value>>(target)) {
            report_error("Function length requires an array as its argument");
            result = {std::nullopt};
            return;
        }

        result = {static_cast<int>(std::get<std::vector<value>>(target).size())};
    } else if (*callee->identifier.payload == "push") {
        if (node.arguments.size() != 2) {
            report_error("Function push requires two arguments");
            result = {std::nullopt};
            return;
        }

        auto* target = get(*node.arguments.front());
        if (!target || !std::holds_alternative<std::vector<value>>(target->val)) {
            report_error("Function push requires an array as its first argument");
            result = {std::nullopt};
            return;
        }

        auto& array = std::get<std::vector<value>>(target->val);
        node.arguments.back()->accept(*this);
        array.push_back(result);
        result = {array};
    } else if (*callee->identifier.payload == "pop") {
        if (node.arguments.size() != 1) {
            report_error("Function push requires two arguments");
            result = {std::nullopt};
            return;
        }

        auto* target = get(*node.arguments.front());
        if (!target || !std::holds_alternative<std::vector<value>>(target->val)) {
            report_error("push requires an array as its first argument");
            result = {std::nullopt};
            return;
        }

        auto& array = std::get<std::vector<value>>(target->val);
        if (!array.empty()) {
            result = {array.back()};
            array.pop_back();
        } else {
            result = {std::nullopt};
        }
    } else {
        report_error(std::string("Unknown function: ") + *callee->identifier.payload);
        result = {std::nullopt};
    }
}

void expression_evaluator::visit(array_literal_expression const& node) {
    std::vector<value> elements;
    elements.reserve(node.elements.size());

    for (auto const& element : node.elements) {
        element->accept(*this);
        elements.push_back({result.val, false});
    }
    result = {elements};
}

void expression_evaluator::visit(array_ref_expression const& node) {
    node.array->accept(*this);
    auto array = result.val;
    if (!std::holds_alternative<std::vector<value>>(array)) {
        report_error("Cannot index into non-array types");
        result = {std::nullopt};
        return;
    }

    auto const& elements = std::get<std::vector<value>>(array);
    node.index->accept(*this);
    auto index = result.val;
    if (!std::holds_alternative<int>(index)) {
        report_error("Array index must be an integer");
        result = {std::nullopt};
        return;
    }

    if (std::get<int>(index) >= elements.size()) {
        report_error("Array index out of bounds");
        result = {std::nullopt};
        return;
    }

    result = elements[std::get<int>(index)];
}

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