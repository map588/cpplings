// intro2.cpp
//
// This one doesn't compile. Two distinct phases of the build are failing,
// and the error messages look different for each:
//
//   1. A *preprocessing/compilation* error: we use a name the compiler has
//      never heard of, because the header that declares it was never
//      #include'd. C++ has no implicit imports — if you use std::cout, you
//      must include <iostream>. (Sometimes code compiles anyway because
//      another header happened to drag the one you need in transitively.
//      Don't rely on that; it breaks when toolchains change.)
//
//   2. A plain *syntax* error.
//
// Rule of thumb: fix the FIRST error the compiler prints, then recompile.
// Later errors are often just the parser falling over downstream of the
// first problem.
//
// Also worth knowing: `main` is special — it's the only function where
// falling off the end without a `return` is well-defined; it implicitly
// returns 0. (True since C++98. For every other non-void function it's
// undefined behavior — which is why this repo compiles with
// -Werror=return-type.)
//
// Task: make the file compile and run.
//   - compiles with no errors, prints "Hello, cpplings!", exits 0
// Constraints:
//   - fix the cout line — don't delete it or switch to a different way
//     of printing


int main() {
    std::cout << "Hello, cpplings!\n"
}
// I AM NOT DONE
