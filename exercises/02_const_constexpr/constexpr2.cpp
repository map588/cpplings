// constexpr2.cpp
//
// C++20 added two sharper tools next to constexpr:
//
//   consteval  — "IMMEDIATE function": every call MUST happen at compile
//                time. Passing it a runtime value is a compile error, not a
//                silent fallback to runtime. Use it when running at runtime
//                would be a bug (key schedules, lookup-table generation...).
//
//   constinit  — promises a static/global is INITIALIZED at compile time,
//                while staying MUTABLE at runtime. This is exactly the combo
//                `constexpr` can't express (constexpr variables are const).
//                Compile-time init means the variable can't suffer the
//                "static initialization order fiasco" — it never depends on
//                which translation unit initializes first.
//
// So for a global:    init time      runtime mutable?
//   const             maybe runtime  no
//   constexpr         compile time   no
//   constinit         compile time   YES
//
// Task: rng_state must start at hash_seed(2026) — computed at compile
// time, GUARANTEED by the compiler — while staying mutable at runtime.
// Give it the one specifier that expresses both. (Try constexpr first
// and read the error.)
//   - compiles; next_random() mutates the global; the assert passes
// Constraints:
//   - keep hash_seed consteval and keep the initializer hash_seed(2026)
//   - a plain non-const global would compile here, but it loses the
//     compile-time-init guarantee this exercise is about — the specifier
//     you pick must make a runtime initializer a compile ERROR

#include <cassert>

// consteval (C++20): this can never run at runtime.
consteval unsigned hash_seed(unsigned year) {
    unsigned h = year;
    h ^= h >> 3;
    h *= 0x9E3779B9u;  // golden-ratio constant, a classic mixing step
    return h;
}

// Compile-time-initialized, runtime-mutable global. Fix the specifier:
const unsigned rng_state = hash_seed(2026);

unsigned next_random() {
    // xorshift step — mutates the global
    unsigned x = rng_state;
    x ^= x << 13;
    x ^= x >> 17;
    x ^= x << 5;
    return rng_state = x;
}

int main() {
    static_assert(hash_seed(2026) != 0);  // consteval result is a constant

    unsigned first = next_random();
    unsigned second = next_random();
    assert(first != second);

    // Uncomment to watch consteval refuse runtime arguments:
    // unsigned y = 2026;
    // unsigned h = hash_seed(y);   // ERROR: y is not a constant expression
    return 0;
}

// I AM NOT DONE
