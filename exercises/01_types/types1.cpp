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
// allow them silently. That compile-time rejection is why modern
// guidelines say "prefer {}". And when you genuinely MEAN to lose
// information, the language expects you to say so in a form the compiler
// (and the next reader) can see is deliberate — not an accident.
//
// Task: make the program compile and pass every assert.
// Constraints:
//   - keep the brace initializers on `loud` and `positive`
//   - do not change any assert line
//   - leave `silent` exactly as it is (it's there for contrast)


#include <cassert>

int main() {
    double ratio = 3.9;

    int silent = ratio;        // legal, lossy, and invisible — the old way
    int loud{ratio};           // the compiler objects — how do you tell it this loss is deliberate?
    unsigned int positive{-1}; // narrowing too: -1 doesn't fit in unsigned

    assert(silent == 3);
    assert(loud == 3);
    assert(positive == 0);     // fix the initializer so this holds
    return 0;
}
// I AM NOT DONE
