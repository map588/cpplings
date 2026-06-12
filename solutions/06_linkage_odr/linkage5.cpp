// linkage5.cpp — solution (the fix is in linkage5_config.h)

#include "linkage5_config.h"

#include <cassert>

const double* other_tu_pi_address();

int main() {
    assert(kPi > 3.14 && kPi < 3.15);
    assert(&kPi == other_tu_pi_address());   // one object now
    return 0;
}
