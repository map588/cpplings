// ctad1.cpp — solution

#include <string>
#include <type_traits>
#include <utility>
#include <vector>

template <typename T>
struct Box {
    T contents;
    explicit Box(T c) : contents(std::move(c)) {}
};

// Deduction guide: construction from a C string means a string Box.
Box(const char*) -> Box<std::string>;

int main() {
    std::pair p{1, 2.5};
    static_assert(std::is_same_v<decltype(p), std::pair<int, double>>);

    std::vector v{1, 2, 3};
    static_assert(std::is_same_v<decltype(v), std::vector<int>>);

    Box number(42);
    static_assert(std::is_same_v<decltype(number), Box<int>>);

    Box greeting("hello");
    static_assert(std::is_same_v<decltype(greeting), Box<std::string>>);
    return 0;
}
