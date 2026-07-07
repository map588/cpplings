// signal1.cpp — solution

#include <cassert>
#include <csignal>       // std::signal, std::raise, std::sig_atomic_t
#include <type_traits>

// The blessed type: writes to it are atomic with respect to signal
// delivery, and `volatile` stops the compiler caching it in a register.
volatile std::sig_atomic_t g_stop = 0;

static_assert(std::is_same_v<decltype(g_stop), volatile std::sig_atomic_t>,
              "a signal handler may only write a volatile "
              "std::sig_atomic_t (or a lock-free std::atomic)");

// Handlers are called by the C runtime, so give them C linkage.
extern "C" void on_shutdown(int) {
    g_stop = 1;    // set the flag, return — nothing else is safe here
}

int main() {
    std::signal(SIGUSR1, on_shutdown);   // the channel raise() uses below

    long iterations = 0;
    while (!g_stop) {
        ++iterations;
        if (iterations == 10'000) {
            std::raise(SIGUSR1);   // handler runs before raise returns
        }
        if (iterations > 10'000) {
            break;
        }
    }

    assert(g_stop == 1);
    assert(iterations == 10'000);
    return 0;
}
