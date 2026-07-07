// const1.cpp
//
// `const` is a promise: "this name will not be used to mutate this object."
// The compiler enforces the promise — and uses it. The function below also
// shows the single most important const habit in C++:
//
//     void f(std::string s)         // copies the whole string. Every call.
//     void f(const std::string& s)  // no copy — read-only view of caller's
//
// Pass-by-const-reference avoids copying anything bigger than a pointer or
// two, while still promising the caller "I won't touch it." For tiny types
// (int, double, pointers) pass by value — a reference is indirection with
// no savings.
//
// This file confuses which things should be const.
//
// Task: fix the declarations so the program compiles and passes.
//   - compiles; every assert passes; exits 0
//   - count_l no longer copies its argument, and still promises not to
//     modify it
// Constraints:
//   - don't weaken `greeting` — the whole point is that it can't change
//   - don't change count_l's body, the call sites, or any assert


#include <cassert>
#include <string>

// This takes the string BY VALUE — a full copy per call. But it only
// ever READS the text...
std::size_t count_l(std::string text) {
    std::size_t n = 0;
    for (char c : text) {
        if (c == 'l') ++n;
    }
    return n;
}

int main() {
    const std::string greeting = "hello, cpplings!";

    // Tally of calls made so far. Someone marked it const — is that
    // promise compatible with what a running tally has to do?
    const int calls = 0;

    assert(count_l(greeting) == 3);
    calls += 1;

    assert(count_l("ll") == 2);
    calls += 1;

    assert(calls == 2);
    return 0;
}
// I AM NOT DONE
