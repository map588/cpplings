// linkage5.cpp        (with linkage5_other.cpp and linkage5_config.h)
//
// Why didn't linkage4's duplicate-symbol disaster ever bite you with
// header constants like this one?
//
//     const double kPi = 3.14159;    // in a header, included everywhere
//
// Because of a C++ special rule: namespace-scope `const` variables have
// INTERNAL linkage by default. (In C they're external! This divergence
// exists precisely so C++ headers could hold constants.) Each TU quietly
// gets its OWN PRIVATE COPY. No symbol clash...
//
// ...and no single object, either. This file's &kPi and the other TU's
// &kPi are DIFFERENT ADDRESSES — run it and watch the assert fail. Same
// value, two objects. That's usually harmless, but it bites when anything
// compares or stores the address: an identity check, a map keyed by
// pointer, an ABI boundary.
//
// C++17 closed the gap: the folding rule you used in linkage4 applies to
// VARIABLES too — one keyword turns a header-defined constant into a
// single program-wide object that every TU shares.
//
// Task: make the program run and exit 0.
//   - both asserts pass: the value is right AND &kPi is the same address
//     in every TU
// Constraints:
//   - the fix goes in linkage5_config.h; kPi stays defined in the header
//   - don't touch linkage5_other.cpp or the asserts

#include "linkage5_config.h"

#include <cassert>

const double* other_tu_pi_address();   // from linkage5_other.cpp

int main() {
    assert(kPi > 3.14 && kPi < 3.15);          // the VALUE was never wrong
    assert(&kPi == other_tu_pi_address());     // the IDENTITY is
    return 0;
}

// I AM NOT DONE
