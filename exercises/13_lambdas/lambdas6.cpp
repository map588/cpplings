// lambdas6.cpp
//
// Storage and cost — the closure is a struct; reason about it like one.
//
//   sizeof: a captureless closure has no members → sizeof == 1 (empty
//           class rule). Capture an int → sizeof == 4. You pay for
//           exactly what you capture.
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
// Task: the offset-aware handler can't be a function pointer — give
// store_handler a parameter type that can hold it (and fix the TODO
// sizes while you're here).

// I AM NOT DONE

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
