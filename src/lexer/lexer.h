#pragma once

#include <iostream>
#include <optional>
#include <vector>

#include "token.h"

namespace rover {
class lexer {
private:
    std::istream& input;
    std::optional<token> peeked;

    std::optional<token> emit(token const& t);

public:
    lexer(std::istream& input);
    std::optional<token> peek();
    std::optional<token> consume();
    std::optional<token> consume_if(std::vector<token_type> const& types);
};

} // namespace rover
