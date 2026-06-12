// lambdas2.cpp — solution

#include <cassert>

int main() {
    int price = 100;

    auto frozen = [price]() { return price; };
    auto live   = [&price]() { return price; };

    price = 250;

    assert(frozen() == 100);    // copied when the lambda was created
    assert(live() == 250);      // reads the original at call time

    // ---- part two -----------------------------------------------------
    int stock = 8;

    // By-reference capture: the closure writes through to `stock`.
    auto record_taking = [&stock](int n) { return stock -= n; };

    assert(record_taking(3) == 5);
    assert(record_taking(2) == 3);
    assert(stock == 3);
    return 0;
}
