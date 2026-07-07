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
// Task: implement median_of() — one constexpr function, both worlds.
//   - the three static_asserts pass: the compiler runs the whole
//     container lifecycle
//   - main's runtime assert passes: same function, runtime data
// Constraints:
//   - use a container as scratch space inside the function — putting
//     the values in order is the library's job, not a hand-rolled
//     selection loop's (every asserted list has odd length; the
//     median is the middle of the ordered values)
//   - no allocation escapes: only the int leaves
//   - don't change the asserts

#include <algorithm>
#include <cassert>
#include <initializer_list>
#include <vector>

constexpr int median_of(std::initializer_list<int> values) {
    return 0;   // TODO: scratch storage, order it, take the middle
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

// I AM NOT DONE
