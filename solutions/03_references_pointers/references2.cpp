// references2.cpp — solution

#include <cassert>
#include <string>

std::string make_greeting() {
    return "hello";
}

// const& : read-only, and temporaries (like string literals) can bind.
std::size_t count_vowels(const std::string& text) {
    std::size_t n = 0;
    for (char c : text) {
        if (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u') ++n;
    }
    return n;
}

int main() {
    // const& binding a temporary directly → lifetime extended to main's end.
    const std::string& greeting = make_greeting();
    assert(greeting == "hello");

    assert(count_vowels(greeting) == 2);
    assert(count_vowels("aeiou") == 5);
    return 0;
}
