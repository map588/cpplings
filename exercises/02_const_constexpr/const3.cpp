// const3.cpp
//
// `const` on a MEMBER FUNCTION promises "calling this does not mutate the
// object." It's part of the function's type — and it's the only thing you
// may call on a const object or through a const reference.
//
// Since pass-by-const-reference is how objects travel through most C++
// APIs, a missing `const` on a getter poisons every caller upstream.
// Mark every member function that doesn't mutate as const, always.
//
// Escape hatch worth knowing: a `mutable` member (C++98) may be modified
// even inside a const member function. Use it for things that aren't part
// of the object's logical state — caches, mutexes, access counters.
//
// Task: print_report takes the Counter by const& (correctly!), but won't
// compile. Decide, member by member, which promises Counter can make.
//   - compiles; the report prints; both asserts pass
// Constraints:
//   - fix the CLASS only — print_report must not change
//   - don't promise anything false: a member function that mutates the
//     object must stay callable, and must keep compiling, exactly as
//     main uses it

#include <cassert>
#include <iostream>

class Counter {
public:
    void increment() { ++count_; }

    int value() { return count_; }

    bool is_even() { return count_ % 2 == 0; }

private:
    int count_ = 0;
};

void print_report(const Counter& c) {
    std::cout << "count=" << c.value()
              << " even=" << c.is_even() << "\n";
}

int main() {
    Counter c;
    c.increment();
    c.increment();
    c.increment();

    print_report(c);
    assert(c.value() == 3);
    assert(!c.is_even());
    return 0;
}

// I AM NOT DONE
