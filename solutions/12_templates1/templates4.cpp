// templates4.cpp — solution

#include <cassert>
#include <string>

template <typename T>
struct Formatter {
    static std::string format(const T& value) {
        return std::to_string(value);
    }
};

// Full specialization: the recipe for exactly bool.
template <>
struct Formatter<bool> {
    static std::string format(bool value) {
        return value ? "yes" : "no";
    }
};

int main() {
    assert(Formatter<int>::format(42) == "42");
    assert(Formatter<double>::format(2.5).substr(0, 3) == "2.5");

    assert(Formatter<bool>::format(true) == "yes");
    assert(Formatter<bool>::format(false) == "no");
    return 0;
}
