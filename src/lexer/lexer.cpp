#include "lexer.h"

#include <algorithm>

namespace rover {
lexer::lexer(std::istream& input) : input(input) {}

std::optional<token> lexer::emit(token const& t) {
    peeked = {t};
    return peeked;
}

std::optional<token> lexer::consume() {
    if (!peeked) {
        peek();
    }

    auto t = peeked;
    peeked = {};
    return t;
}

std::optional<token> lexer::consume_if(std::vector<token_type> const& types) {
    if (!peeked) {
        peek();
    }

    if (peeked && std::count_if(types.begin(), types.end(), [this](auto t) { return peeked->type == t; }) > 0) {
        auto t = peeked;
        peeked = {};
        return t;
    }

    return {};
}

std::optional<token> lexer::peek() {
    if (peeked) {
        return peeked;
    }

    if (!input) {
        return emit(token{token_type::END_OF_FILE, {}});
    }

    char c;
    while (input.get(c) && std::isspace(c))
        ;

    if (!input) {
        return emit(token{token_type::END_OF_FILE, {}});
    }

    if (std::isalpha(c)) {
        std::string s;
        s += c;
        input.get(c);

        while (std::isalnum(c) || c == '_') {
            s += c;
            input.get(c);
        }
        input.unget();

        if (s == "if") {
            return emit(token{token_type::IF, {}});
        } else if (s == "else") {
            return emit(token{token_type::ELSE, {}});
        } else if (s == "while") {
            return emit(token{token_type::WHILE, {}});
        } else if (s == "var") {
            return emit(token{token_type::VAR, {}});
        } else if (s == "const") {
            return emit(token{token_type::CONST, {}});
        } else {
            return emit(token{token_type::IDENTIFIER, {s}});
        }
    } else if (std::isdigit(c)) {
        std::string s;
        s += c;

        while (input.get(c) && std::isdigit(c)) {
            s += c;
        }

        if (c == '.') {
            s += c;
            while (input.get(c) && std::isdigit(c)) {
                s += c;
            }
            input.unget();

            return emit(token{token_type::FLOAT, s});
        } else {
            input.unget();
            return emit(token{token_type::INT, s});
        }
    } else if (c == '\0') {
        return emit(token{token_type::END_OF_FILE, {}});
    } else if (c == '+') {
        return emit(token{token_type::PLUS, {}});
    } else if (c == '-') {
        return emit(token{token_type::MINUS, {}});
    } else if (c == '*') {
        return emit(token{token_type::STAR, {}});
    } else if (c == '/') {
        char next;
        input.get(next);
        if (next == '/') {
            while (input.get(next) && next != '\n')
                ;
            return peek();
        } else {
            input.unget();
            return emit(token{token_type::SLASH, {}});
        }
    } else if (c == '=') {
        char next;
        input.get(next);
        if (next == '=') {
            return emit(token{token_type::EQUAL, {}});
        } else {
            input.unget();
            return emit(token{token_type::ASSIGN, {}});
        }
    } else if (c == '!') {
        char next;
        input.get(next);
        if (next == '=') {
            return emit(token{token_type::NOT_EQUAL, {}});
        } else {
            input.unget();
            return emit(token{token_type::NOT, {}});
        }
    } else if (c == '<') {
        char next;
        input.get(next);
        if (next == '=') {
            return emit(token{token_type::LESS_EQUAL, {}});
        } else {
            input.unget();
            return emit(token{token_type::LESS_THAN, {}});
        }
        return emit(token{token_type::LESS_THAN, {}});
    } else if (c == '>') {
        char next;
        input.get(next);
        if (next == '=') {
            return emit(token{token_type::GREATER_EQUAL, {}});
        } else {
            input.unget();
            return emit(token{token_type::GREATER_THAN, {}});
        }
    } else if (c == '{') {
        return emit(token{token_type::LEFT_BRACE, {}});
    } else if (c == '}') {
        return emit(token{token_type::RIGHT_BRACE, {}});
    } else if (c == '(') {
        return emit(token{token_type::LEFT_PAREN, {}});
    } else if (c == ')') {
        return emit(token{token_type::RIGHT_PAREN, {}});
    } else if (c == '[') {
        return emit(token{token_type::LEFT_SQUARE, {}});
    } else if (c == ']') {
        return emit(token{token_type::RIGHT_SQUARE, {}});
    } else if (c == ',') {
        return emit(token{token_type::COMMA, {}});
    } else if (c == ';') {
        return emit(token{token_type::SEMICOLON, {}});
    } else if (c == '"') {
        std::string str;
        while (input.get(c) && c != '"') {
            str += c;
        }
        return emit(token{token_type::STRING, {str}});
    }
    return {};
}
} // namespace rover