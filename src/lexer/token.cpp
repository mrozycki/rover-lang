#include "token.h"

namespace rover {
std::ostream& operator<<(std::ostream& os, const token& t) {
    switch (t.type) {
        case token_type::IF:
            os << "IF";
            break;
        case token_type::ELSE:
            os << "ELSE";
            break;
        case token_type::WHILE:
            os << "WHILE";
            break;
        case token_type::VAR:
            os << "VAR";
            break;
        case token_type::CONST:
            os << "CONST";
            break;
        case token_type::IDENTIFIER:
            os << "IDENTIFIER(" << t.payload.value() << ")";
            break;
        case token_type::INT:
            os << "INT(" << t.payload.value() << ")";
            break;
        case token_type::FLOAT:
            os << "FLOAT(" << t.payload.value() << ")";
            break;
        case token_type::STRING:
            os << "STRING(" << t.payload.value() << ")";
            break;
        case token_type::LEFT_BRACE:
            os << "LEFT_BRACE";
            break;
        case token_type::RIGHT_BRACE:
            os << "RIGHT_BRACE";
            break;
        case token_type::LEFT_PAREN:
            os << "LEFT_PAREN";
            break;
        case token_type::RIGHT_PAREN:
            os << "RIGHT_PAREN";
            break;
        case token_type::LEFT_SQUARE:
            os << "LEFT_SQUARE";
            break;
        case token_type::RIGHT_SQUARE:
            os << "RIGHT_SQUARE";
            break;
        case token_type::COMMA:
            os << "COMMA";
            break;
        case token_type::SEMICOLON:
            os << "SEMICOLON";
            break;
        case token_type::ASSIGN:
            os << "ASSIGN";
            break;
        case token_type::PLUS:
            os << "PLUS";
            break;
        case token_type::MINUS:
            os << "MINUS";
            break;
        case token_type::STAR:
            os << "STAR";
            break;
        case token_type::SLASH:
            os << "SLASH";
            break;
        case token_type::EQUAL:
            os << "EQUAL";
            break;
        case token_type::NOT_EQUAL:
            os << "NOT_EQUAL";
            break;
        case token_type::LESS_THAN:
            os << "LESS_THAN";
            break;
        case token_type::LESS_EQUAL:
            os << "LESS_EQUAL";
            break;
        case token_type::GREATER_THAN:
            os << "GREATER_THAN";
            break;
        case token_type::GREATER_EQUAL:
            os << "GREATER_EQUAL";
            break;
        case token_type::NOT:
            os << "NOT";
            break;
        case token_type::AND:
            os << "AND";
            break;
        case token_type::OR:
            os << "OR";
            break;
        case token_type::END_OF_FILE:
            os << "END_OF_FILE";
            break;
    }
    return os;
}
} // namespace rover