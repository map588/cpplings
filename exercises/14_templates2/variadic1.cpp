// variadic1.cpp
//
// A PARAMETER PACK accepts any number of arguments of any types:
//
//     template <typename... Ts>        // Ts: a pack of types
//     auto sum(Ts... args) {           // args: a pack of values
//
// Two tools work on packs:
//
//   sizeof...(args)    — how many (a compile-time constant)
//
//   FOLD EXPRESSIONS (C++17) — apply a binary operator across the pack:
//     (args * ...)             // a1 * (a2 * (a3 * ...))   unary fold
//     (1 * ... * args)         // binary fold: the seed value makes the
//                              // EMPTY pack legal, and defines its value
//     (args || ...)            // logic folds too; && and || even have
//                              // built-in empty-pack answers (true, false)
//
// The operand may be any expression that mentions the pack — not just
// the bare name.
//
// Before C++17 every variadic function was a recursion: peel one
// argument, recurse on the rest, write a base case. Folds deleted ~90%
// of that. (You'll still write the recursive form when per-element
// logic gets complex — variadic2 shows the modern alternative there
// too.)
//
// Task: implement the three TODO bodies so every assert and
// static_assert passes.
//   - the EMPTY calls must work too: sum() == 0, count_args() == 0,
//     all_positive() == true — let that drive your choice of fold form
// Constraints:
//   - no recursion, no loops, no helper overloads — each body is a
//     single return statement
//   - do not change the asserts

#include <cassert>
#include <cstddef>

template <typename... Ts>
auto sum(Ts... args) {
    return 0;   // TODO
}

template <typename... Ts>
constexpr std::size_t count_args(Ts... args) {
    return 0;   // TODO
}

template <typename... Ts>
bool all_positive(Ts... args) {
    return false;   // TODO
}

int main() {
    assert(sum(1, 2, 3, 4) == 10);
    assert(sum(0.5, 2) == 2.5);          // mixed types: usual conversions
    assert(sum() == 0);                  // empty pack

    static_assert(count_args(9, 9, 9) == 3);
    static_assert(count_args() == 0);

    assert(all_positive(1, 2, 3));
    assert(!all_positive(1, -2, 3));
    assert(all_positive());              // vacuous truth: && folds to true
    return 0;
}

// I AM NOT DONE
