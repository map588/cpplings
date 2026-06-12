// conc6.cpp — solution

#include <atomic>
#include <cassert>
#include <thread>

int payload = 0;
std::atomic<bool> ready{false};

void writer() {
    payload = 42;
    ready.store(true, std::memory_order_release);   // publish: everything
}                                                   // above is included

void reader(int& out) {
    while (!ready.load(std::memory_order_acquire)) {   // receive: see all
    }                                                  // the writer did
    out = payload;                                     // before its release
}

int main() {
    int received = 0;
    {
        std::jthread r(reader, std::ref(received));
        std::jthread w(writer);
    }
    assert(received == 42);
    return 0;
}
