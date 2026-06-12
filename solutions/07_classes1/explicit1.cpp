// explicit1.cpp — solution

#include <cassert>

struct Seconds {
    int n;
    explicit Seconds(int n) : n(n) {}
};

struct Minutes {
    int n;
    explicit Minutes(int n) : n(n) {}
};

int to_total_seconds(Seconds s, Minutes m) {
    return s.n + m.n * 60;
}

int main() {
    // The units are now spelled out — and un-swappable:
    int total = to_total_seconds(Seconds{30}, Minutes{5});
    assert(total == 330);
    return 0;
}
