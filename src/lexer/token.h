#pragma once

#include <iostream>
#include <optional>
#include <string>

namespace rover {
enum class token_type {
    IF,
    ELSE,
    WHILE,
    VAR,
    CONST,
    IDENTIFIER,
    INT,
    FLOAT,
    STRING,
    LEFT_BRACE,
    RIGHT_BRACE,
    LEFT_PAREN,
    RIGHT_PAREN,
    LEFT_SQUARE,
    RIGHT_SQUARE,
    COMMA,
    SEMICOLON,
    ASSIGN,
    PLUS,
    MINUS,
    STAR,
    SLASH,
    EQUAL,
    NOT_EQUAL,
    LESS_THAN,
    LESS_EQUAL,
    GREATER_THAN,
    GREATER_EQUAL,
    NOT,
    AND,
    OR,
    END_OF_FILE
};

struct token {
    token_type type;
    std::optional<std::string> payload;
};

std::ostream& operator<<(std::ostream& os, const token& t);
} // namespace rover