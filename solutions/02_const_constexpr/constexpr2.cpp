// constexpr2.cpp — solution

#include <cassert>

consteval unsigned hash_seed(unsigned year) {
    unsigned h = year;
    h ^= h >> 3;
    h *= 0x9E3779B9u;
    return h;
}

// constinit (C++20): initialized at compile time, mutable at runtime.
// (constexpr would also pin init to compile time, but makes it const.)
constinit unsigned rng_state = hash_seed(2026);

unsigned next_random() {
    unsigned x = rng_state;
    x ^= x << 13;
    x ^= x >> 17;
    x ^= x << 5;
    return rng_state = x;
}

int main() {
    static_assert(hash_seed(2026) != 0);

    unsigned first = next_random();
    unsigned second = next_random();
    assert(first != second);
    return 0;
}
