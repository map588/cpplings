// const3.cpp — solution

#include <cassert>
#include <iostream>

class Counter {
public:
    void increment() { ++count_; }  // mutates: correctly NOT const

    int value() const { return count_; }

    bool is_even() const { return count_ % 2 == 0; }

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
