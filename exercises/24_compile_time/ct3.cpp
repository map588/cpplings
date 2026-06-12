// ct3.cpp
//
// C++20 made std::vector and std::string usable IN CONSTANT EVALUATION:
// allocate, grow, sort, free — all inside the compiler. One rule: no
// allocation may ESCAPE to runtime. Use the vector as scratch space,
// return a count / an array / a bool.
//
//     constexpr int distinct_words(std::string_view text) {
//         std::vector<std::string_view> seen;     // compile-time vector!
//         ...
//         return seen.size();                     // the int escapes;
//     }                                           // the vector doesn't
//     static_assert(distinct_words("a b a") == 2);
//
// Suddenly "parse the config grammar at build time" or "validate the
// opcode table before the binary exists" is ordinary code — same
// functions, both worlds (every constexpr function here still works on
// runtime data too).
//
// Task: implement median_of() — constexpr, takes an initializer_list,
// copies into a std::vector, sorts (std::sort is constexpr in C++20),
// returns the middle element. Compile-time callers below, runtime
// caller at the bottom.

// I AM NOT DONE

#include <algorithm>
#include <cassert>
#include <initializer_list>
#include <vector>

constexpr int median_of(std::initializer_list<int> values) {
    return 0;   // TODO: vector scratch + std::sort + middle element
}

// The compiler runs the whole vector lifecycle for these:
static_assert(median_of({3, 1, 2}) == 2);
static_assert(median_of({9, 4, 7, 1, 8}) == 7);
static_assert(median_of({42}) == 42);

int main() {
    // Same function, runtime data:
    int sensor_a = 18, sensor_b = 4, sensor_c = 11;
    assert(median_of({sensor_a, sensor_b, sensor_c}) == 11);
    return 0;
}
