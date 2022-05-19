#pragma once

#include <optional>
#include <string>

#include <unordered_map>

#include "value.h"

namespace rover {
class context {
private:
    std::unordered_map<std::string, value> variables;
    context* parent;

public:
    context(context* parent_);

    void set(std::string const& name, value v);
    bool update(std::string const& name, value v);
    std::optional<value> get(std::string const& name) const;
};
} // namespace rover