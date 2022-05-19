#pragma once

#include <optional>
#include <string>
#include <variant>
#include <vector>

namespace rover {
struct value {
    std::variant<int, double, std::string, std::vector<value>, std::nullopt_t> val;
    bool is_const;
};
} // namespace rover
