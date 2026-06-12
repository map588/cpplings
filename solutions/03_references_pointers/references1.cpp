// references1.cpp — solution

#include <cassert>

int main() {
    int a = 1;
    int b = 2;

    int& ra = a;   // bound at birth, aliases a forever

    int& rb = b;
    rb = a;        // assigns a's VALUE (1) to b — no rebinding

    a = 100;

    assert(b == 1);       // got a's value at the time of assignment
    assert(rb == 1);      // rb still aliases b
    assert(&rb == &b);
    assert(ra == 100);    // ra aliases a
    return 0;
}
