// lambdas3.cpp — solution

#include <cassert>

int main() {
    // mutable: operator() may modify the closure's members.
    auto next_id = [id = 100]() mutable { return ++id; };

    assert(next_id() == 101);
    assert(next_id() == 102);

    auto fresh_copy = next_id;        // copy made while id == 102

    assert(next_id() == 103);
    assert(fresh_copy() == 103);      // independent state: 102 + 1
    return 0;
}
