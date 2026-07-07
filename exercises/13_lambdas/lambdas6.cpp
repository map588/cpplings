// lambdas6.cpp
//
// Storage and cost — the closure is a struct; reason about it like one.
//
//   sizeof: the closure is a struct, so ordinary layout rules apply. A
//           captureless closure has no members — recall what C++ says an
//           empty class's size must be. Capture an int and the closure
//           is exactly as big as its one member. You pay for precisely
//           what you capture.
//
//   function pointers: a CAPTURELESS lambda converts to a plain
//           C-style function pointer (there's no state to smuggle).
//           One capture and that conversion is gone — where would the
//           state live in a bare pointer?
//
//   std::function<int(int)> (C++11): TYPE ERASURE. It can hold ANY
//           callable with that signature — lambda, function pointer,
//           functor — at the price of an indirect call, a fatter object,
//           and a possible heap allocation for big closures. It's the
//           right tool for STORING heterogeneous callbacks; it's the
//           wrong default for passing a lambda straight into an
//           algorithm (templates/auto take the closure type directly,
//           inlinably — module 12's way).
//
// Task: fill in the TODO sizes, and make the handler slot able to hold
// the offset-aware handler too.
//   - both static_asserts and both asserts pass
// Constraints:
//   - both lambdas stay exactly as written (offsetter keeps its capture)
//   - keep the single global slot plus store_handler — heterogeneous
//     callables must flow through the same slot at runtime
//   - on the static_assert lines, only the TODOs change

#include <cassert>
#include <cstddef>
#include <functional>

constexpr std::size_t TODO = 9999;

int (*g_handler)(int) = nullptr;

void store_handler(int (*h)(int)) {
    g_handler = h;
}

int main() {
    auto doubler = [](int x) { return 2 * x; };

    int offset = 100;
    auto offsetter = [offset](int x) { return x + offset; };

    // You pay for what you capture:
    static_assert(sizeof(doubler) == TODO);      // empty class
    static_assert(sizeof(offsetter) == TODO);    // one int member

    store_handler(doubler);          // captureless → function pointer: OK
    assert(g_handler(21) == 42);

    store_handler(offsetter);        // where would `offset` live in a
    assert(g_handler(1) == 101);     // bare function pointer?
    return 0;
}

// I AM NOT DONE
