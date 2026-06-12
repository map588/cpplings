// functions3.cpp — solution

#include <cassert>

int next_ticket() {
    static int counter = 0;   // initialized once, lives until program exit
    return ++counter;
}

int main() {
    assert(next_ticket() == 1);
    assert(next_ticket() == 2);
    assert(next_ticket() == 3);
    return 0;
}
