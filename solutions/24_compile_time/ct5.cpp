// ct5.cpp — solution

#include <cassert>
#include <type_traits>

constexpr int square(int x, char* mode = nullptr) {
    if (std::is_constant_evaluated()) {    // PLAIN if: asked at the right time
        if (mode) *mode = 'c';
        return x * x;
    } else {
        if (mode) *mode = 'r';
        return x * x;
    }
}

int main() {
    static_assert(square(12) == 144);

    char mode = '?';
    int runtime_x = 5;
    assert(square(runtime_x, &mode) == 25);
    assert(mode == 'r');                   // runtime call, runtime path
    return 0;
}
