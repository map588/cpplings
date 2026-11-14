// conc8.cpp — solution

#include <atomic>
#include <cassert>
#include <chrono>
#include <thread>

std::atomic<int> tickets{1};
std::atomic<int> sold{0};

bool try_buy() {
    int seen = tickets.load();                                       // look
    while (seen > 0) {
        std::this_thread::sleep_for(std::chrono::milliseconds(50));  // the gap
        // Commit only if the count is still what we saw. On failure the
        // CAS has put the current count into `seen`: the loop decides again.
        if (tickets.compare_exchange_weak(seen, seen - 1)) {
            return true;
        }
    }
    return false;
}

void buyer() {
    if (try_buy()) {
        sold.fetch_add(1);
    }
}

int main() {
    {
        std::jthread a(buyer);
        std::jthread b(buyer);
    }
    assert(sold == 1);
    assert(tickets == 0);
    return 0;
}
