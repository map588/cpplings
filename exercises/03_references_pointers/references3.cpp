// references3.cpp
//
// Choosing a parameter type is an API design decision:
//
//   void f(T x)          — a COPY. Caller's object untouched. Right for
//                          cheap types (int, double, pointers, string_view).
//   void f(const T& x)   — read-only access, no copy. Right for expensive
//                          types you only look at.
//   void f(T& x)         — the function MUTATES the caller's object.
//                          The signature is documentation: "this is an
//                          in/out parameter."
//
// (There's a fourth, T&&, in the move-semantics module.)
//
// Task: implement the two TODO functions. The asserts define the contract.
//   - compiles; every assert passes; exits 0
// Constraints:
//   - swap_ints: keep the given signature; don't call std::swap — build
//     it yourself once
//   - shout: pick the parameter type from the table above. "In place"
//     means the caller's own string gains the '!' — no copies
//   - don't change main or the asserts

#include <cassert>
#include <string>

// Swap the values of a and b — the oldest reference exercise in the book.
// (std::swap exists, of course; build it yourself once.)
void swap_ints(int& a, int& b) {
    // TODO
}

// Append "!" to text, in place, and return how long it became.
// Pick the parameter type yourself — the contract says "in place".
std::size_t shout(/* TODO */) {
    // TODO
    return 0;
}

int main() {
    int x = 1, y = 2;
    swap_ints(x, y);
    assert(x == 2 && y == 1);

    std::string word = "go";
    std::size_t len = shout(word);
    assert(word == "go!");   // mutated in place — no copy involved
    assert(len == 3);
    return 0;
}

// I AM NOT DONE
