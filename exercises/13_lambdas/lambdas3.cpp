// lambdas3.cpp
//
// The generated operator() is CONST by default — value-captured state is
// frozen. To get a stateful lambda you need two pieces:
//
//   INIT-CAPTURE (C++14):  [count = 0]
//       a member with an initializer — it doesn't have to copy an outer
//       variable; it can be born inside the closure. (Also the way to
//       MOVE into a closure: [s = std::move(big)].)
//
//   mutable:  [count = 0]() mutable { return ++count; }
//       removes the const from operator(), letting the body modify the
//       closure's members. (The keyword from module 02, third meaning.)
//
// And because the state is an ordinary member: COPYING the closure
// copies the state. Two copies = two independent counters. (Want shared
// state? Capture a reference — or a shared_ptr, module 10.)
//
// Task: make next_id compile and behave, then predict the copied
// closure's output.

#include <cassert>

constexpr int TODO = -1;

int main() {
    // A ticket dispenser in one line — when it compiles:
    auto next_id = [id = 100]() { return ++id; };

    assert(next_id() == 101);
    assert(next_id() == 102);

    auto fresh_copy = next_id;        // copies the closure — and its id

    assert(next_id() == 103);         // the original keeps counting
    assert(fresh_copy() == TODO);     // what did the copy take with it?
    return 0;
}

// I AM NOT DONE
