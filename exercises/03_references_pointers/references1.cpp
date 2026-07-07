// references1.cpp
//
// A reference is an ALIAS for an existing object. Three consequences:
//
//   1. It must be initialized — an alias for nothing is meaningless.
//        int& r;          // ERROR
//   2. It can never be reseated. Assigning to a reference assigns
//      THROUGH it, to the object it aliases.
//   3. After `int& ra = a;` there is no independent "ra object" you can
//      point at — &ra is literally &a.
//
// Pointer people misread rule 2 constantly, so prove it to yourself here.
//
// Task: fix the broken declaration, then replace each TODO with the value
// the program actually produces. Predict before you run.
//   - compiles; every assert passes; exits 0
// Constraints:
//   - ra must end up as a reference that aliases `a` (no pointer, no
//     copy, no rebinding to something more convenient)
//   - don't change any assert — `&rb == &b` stays as the proof

#include <cassert>

constexpr int TODO = -1;

int main() {
    int a = 1;
    int b = 2;

    int& ra;       // ERROR: a reference must alias something from birth
    ra = a;        // ...and this line would never "bind" it — it assigns.

    int& rb = b;
    rb = a;        // this does NOT make rb alias a. What does it do?

    a = 100;

    assert(b == TODO);    // what did `rb = a` do to b?
    assert(rb == TODO);   // rb still aliases b
    assert(&rb == &b);    // the alias never moved
    assert(ra == TODO);   // ra aliases a, which is now...
    return 0;
}

// I AM NOT DONE
