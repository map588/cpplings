// types1.cpp
//
// C++ has (at least) three ways to initialize a variable:
//
//   int a = 5;    // copy-initialization      (since C)
//   int b(5);     // direct-initialization    (since C++98)
//   int c{5};     // list-initialization      (C++11, "uniform"/"brace" init)
//
// They are NOT equivalent. The big one: brace init refuses *narrowing
// conversions* — conversions that can lose information. The older forms
// allow them silently:
//
//   int x = 3.9;   // compiles. x == 3. The .9 is silently discarded.
//   int y{3.9};    // ERROR: narrowing conversion of double to int
//
// This compile-time rejection is why modern guidelines say "prefer {}".
// If you *mean* to truncate, say so explicitly with static_cast — then the
// reader knows it's intentional, not an accident.
//
// Task: make this compile WITHOUT changing the brace-init style and without
// changing what the asserts expect. Express the truncations explicitly.


#include <cassert>

int main() {
    double ratio = 3.9;

    int silent = ratio;        // legal, lossy, and invisible — the old way
    int loud{ratio};           // the compiler objects. Make the intent explicit.
    unsigned int positive{-1}; // narrowing too: -1 doesn't fit in unsigned

    assert(silent == 3);
    assert(loud == 3);
    assert(positive == 0);     // fix the initializer so this holds
    return 0;
}
// I AM NOT DONE
