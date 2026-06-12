// conc2.cpp — solution

#include <atomic>
#include <cassert>
#include <thread>

std::atomic<int> hits{0};

void hammer() {
    for (int i = 0; i < 100'000; ++i) {
        ++hits;                  // indivisible read-modify-write
    }
}

int main() {
    {
        std::jthread a(hammer);
        std::jthread b(hammer);
    }

    assert(hits == 200'000);
    return 0;
}
