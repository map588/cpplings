// functions4.cpp — solution

#include <cassert>
#include <utility>

char pick(int&)       { return 'l'; }
char pick(const int&) { return 'c'; }
char pick(int&&)      { return 'r'; }

int main() {
    int x = 1;
    const int cx = 2;

    assert(pick(x) == 'l');               // mutable lvalue
    assert(pick(cx) == 'c');              // const lvalue
    assert(pick(5) == 'r');               // literal = rvalue
    assert(pick(x + 1) == 'r');           // the result of + is a temporary
    assert(pick(std::move(x)) == 'r');    // move = cast to rvalue ref

    // Thought experiment: without the int&& overload, every 'r' above
    // becomes 'c' — const int& happily binds rvalues. That fallback is
    // why code written before C++11 kept working when move semantics
    // arrived: const& functions accept moved-from arguments fine, they
    // just copy instead of move.
    return 0;
}
