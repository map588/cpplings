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
// Task: this file confuses which things should be const. Fix the
// declarations so it compiles AND no longer copies the big string. Don't
// weaken `greeting` — the whole point is that it can't change.


#include <cassert>
#include <string>

// This takes the string BY VALUE — a full copy per call. Make it take a
// const reference instead.
std::size_t count_l(std::string text) {
    std::size_t n = 0;
    for (char c : text) {
        if (c == 'l') ++n;
    }
    return n;
}

int main() {
    const std::string greeting = "hello, cpplings!";

    // Tally of calls made so far. Someone marked it const, but a tally
    // must change — const is wrong HERE (and right on `greeting`).
    const int calls = 0;

    assert(count_l(greeting) == 3);
    calls += 1;

    assert(count_l("ll") == 2);
    calls += 1;

    assert(calls == 2);
    return 0;
}
// I AM NOT DONE
