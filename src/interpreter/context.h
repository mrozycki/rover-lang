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

    void set(std::string const& name, value const& v);
    bool update(std::string const& name, value const& v);
    std::optional<value> get(std::string const& name);
    value* get_ptr(std::string const& name);
};
} // namespace rover