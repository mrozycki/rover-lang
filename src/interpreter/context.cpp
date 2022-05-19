#include <context.h>

namespace rover {
context::context(context* parent_) : parent(parent_) {}

void context::set(std::string const& name, value const& v) { variables[name] = v; }

bool context::update(std::string const& name, value const& v) {
    auto it = variables.find(name);
    if (it == variables.end()) {
        if (parent) {
            return parent->update(name, v);
        } else {
            return false;
        }
    }

    if (it->second.is_const) {
        return false;
    }

    it->second = v;
    return true;
}

std::optional<value> context::get(std::string const& name) {
    if (auto ptr = get_ptr(name)) {
        return *ptr;
    } else {
        return std::nullopt;
    }
}

value* context::get_ptr(std::string const& name) {
    auto it = variables.find(name);
    if (it != variables.end()) {
        return &(it->second);
    } else if (parent) {
        return parent->get_ptr(name);
    } else {
        return nullptr;
    }
}
} // namespace rover