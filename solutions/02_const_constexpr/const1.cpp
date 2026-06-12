// const1.cpp — solution

#include <cassert>
#include <string>

// const& : no copy, and a promise not to modify the caller's string.
std::size_t count_l(const std::string& text) {
    std::size_t n = 0;
    for (char c : text) {
        if (c == 'l') ++n;
    }
    return n;
}

int main() {
    const std::string greeting = "hello, cpplings!";

    int calls = 0;  // a tally mutates: not const

    assert(count_l(greeting) == 3);
    calls += 1;

    assert(count_l("ll") == 2);
    calls += 1;

    assert(calls == 2);
    return 0;
}
