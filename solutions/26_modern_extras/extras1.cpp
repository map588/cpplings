// extras1.cpp — solution

#include <algorithm>
#include <cassert>
#include <chrono>

using namespace std::chrono_literals;

std::chrono::milliseconds retry_delay(int attempt) {
    auto delay = 100ms * (1 << attempt);
    return std::min<std::chrono::milliseconds>(delay, 3s);   // 3s converts
}                                                            // losslessly

int main() {
    assert(retry_delay(0) == 100ms);
    assert(retry_delay(1) == 200ms);
    assert(retry_delay(3) == 800ms);

    assert(retry_delay(10) == 3s);

    auto precise = 2500ms;
    auto coarse = std::chrono::duration_cast<std::chrono::seconds>(precise);
    assert(coarse == 2s);
    assert(coarse.count() == 2);
    return 0;
}
