#include "lexer.h"

#include <algorithm>

namespace rover {
lexer::lexer(std::istream& input) : input(input), line(1), column(0) {}

std::optional<token> lexer::emit(token const& t) {
    peeked = {t};
    return peeked;
}

std::basic_istream<char>& lexer::get(char& c) {
    auto& ret = input.get(c);

    if (c == '\n') {
        column = 0;
        ++line;
    } else {
        ++column;
    }

    return ret;
}

std::basic_istream<char>& lexer::unget() {
    auto& ret = input.unget();

    --column;

    if (column < 0) {
        column = 0;
        --line;
    }

    return ret;
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
        return emit(token{token_type::END_OF_FILE, line, column, {}});
    }

    char c;
    while (get(c) && std::isspace(c))
        ;

    if (!input) {
        return emit(token{token_type::END_OF_FILE, {}});
    }

    auto start_column = column;

    if (std::isalpha(c)) {
        std::string s;
        s += c;
        get(c);

        while (std::isalnum(c) || c == '_') {
            s += c;
            get(c);
        }
        unget();

        if (s == "if") {
            return emit(token{token_type::IF, line, start_column, {}});
        } else if (s == "else") {
            return emit(token{token_type::ELSE, line, start_column, {}});
        } else if (s == "while") {
            return emit(token{token_type::WHILE, line, start_column, {}});
        } else if (s == "var") {
            return emit(token{token_type::VAR, line, start_column, {}});
        } else if (s == "const") {
            return emit(token{token_type::CONST, line, start_column, {}});
        } else {
            return emit(token{token_type::IDENTIFIER, line, start_column, {s}});
        }
    } else if (std::isdigit(c)) {
        std::string s;
        s += c;

        while (get(c) && std::isdigit(c)) {
            s += c;
        }

        if (c == '.') {
            s += c;
            while (get(c) && std::isdigit(c)) {
                s += c;
            }
            unget();

            return emit(token{token_type::FLOAT, line, start_column, s});
        } else {
            unget();
            return emit(token{token_type::INT, line, start_column, s});
        }
    } else if (c == '\0') {
        return emit(token{token_type::END_OF_FILE, line, start_column, {}});
    } else if (c == '+') {
        return emit(token{token_type::PLUS, line, start_column, {}});
    } else if (c == '-') {
        return emit(token{token_type::MINUS, line, start_column, {}});
    } else if (c == '*') {
        return emit(token{token_type::STAR, line, start_column, {}});
    } else if (c == '/') {
        char next;
        get(next);
        if (next == '/') {
            while (get(next) && next != '\n')
                ;
            return peek();
        } else {
            unget();
            return emit(token{token_type::SLASH, line, start_column, {}});
        }
    } else if (c == '=') {
        char next;
        get(next);
        if (next == '=') {
            return emit(token{token_type::EQUAL, line, start_column, {}});
        } else {
            unget();
            return emit(token{token_type::ASSIGN, line, start_column, {}});
        }
    } else if (c == '!') {
        char next;
        get(next);
        if (next == '=') {
            return emit(token{token_type::NOT_EQUAL, line, start_column, {}});
        } else {
            unget();
            return emit(token{token_type::NOT, line, start_column, {}});
        }
    } else if (c == '<') {
        char next;
        get(next);
        if (next == '=') {
            return emit(token{token_type::LESS_EQUAL, line, start_column, {}});
        } else {
            unget();
            return emit(token{token_type::LESS_THAN, line, start_column, {}});
        }
    } else if (c == '>') {
        char next;
        get(next);
        if (next == '=') {
            return emit(token{token_type::GREATER_EQUAL, line, start_column, {}});
        } else {
            unget();
            return emit(token{token_type::GREATER_THAN, line, start_column, {}});
        }
    } else if (c == '{') {
        return emit(token{token_type::LEFT_BRACE, line, start_column, {}});
    } else if (c == '}') {
        return emit(token{token_type::RIGHT_BRACE, line, start_column, {}});
    } else if (c == '(') {
        return emit(token{token_type::LEFT_PAREN, line, start_column, {}});
    } else if (c == ')') {
        return emit(token{token_type::RIGHT_PAREN, line, start_column, {}});
    } else if (c == '[') {
        return emit(token{token_type::LEFT_SQUARE, line, start_column, {}});
    } else if (c == ']') {
        return emit(token{token_type::RIGHT_SQUARE, line, start_column, {}});
    } else if (c == ',') {
        return emit(token{token_type::COMMA, line, start_column, {}});
    } else if (c == ';') {
        return emit(token{token_type::SEMICOLON, line, start_column, {}});
    } else if (c == '"') {
        std::string str;
        while (get(c) && c != '"') {
            str += c;
        }
        return emit(token{token_type::STRING, line, start_column, {str}});
    }
    return {};
}
} // namespace rover