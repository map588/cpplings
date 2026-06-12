// ifconstexpr1.cpp — solution

#include <cassert>
#include <string>
#include <type_traits>

template <typename T>
std::string to_display(T value) {
    if constexpr (std::is_pointer_v<T>) {
        if (value == nullptr) return "(null)";    // a RUNTIME if, inside
        return "-> " + std::to_string(*value);
    } else if constexpr (std::is_same_v<T, bool>) {
        return value ? "true" : "false";
    } else {
        return std::to_string(value);
    }
}

int main() {
    assert(to_display(42) == "42");
    assert(to_display(true) == "true");

    int x = 7;
    assert(to_display(&x) == "-> 7");
    assert(to_display(static_cast<int*>(nullptr)) == "(null)");
    return 0;
}
