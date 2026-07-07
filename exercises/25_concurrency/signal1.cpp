// signal1.cpp        (single-threaded on purpose — plain ASan, no TSan)
//
// (Signals live in standard C's <csignal>, but SIGUSR1 and the delivery
// semantics described here are POSIX.)
//
// A signal handler is not a thread. When the kernel delivers a signal,
// YOUR thread is suspended between two machine instructions — mid-
// increment, mid-malloc, mid-anything — the handler runs on top of that
// half-done state, then the thread resumes as if nothing happened.
// That's REENTRANCY, not concurrency, and it makes handlers the most
// restricted code you will ever write. Inside one you may only:
//
//   - assign to a `volatile std::sig_atomic_t` (the C-era type the
//     standard blesses for exactly this) or to a lock-free
//     std::atomic (C++11), and
//   - call async-signal-safe functions — a short POSIX list (write,
//     _exit, ...). NOT printf, NOT malloc, NOT cout: any of those may
//     take a lock the interrupted thread is already holding. Deadlock,
//     occasionally, in production, never in the demo.
//
// So real handlers do exactly one thing: set a flag and return; the
// main loop notices. That is the shutdown pattern of every Linux daemon
// (systemd sends a signal; you finish the request, flush, exit) and the
// ISR-flag pattern of every firmware main loop.
//
// The daemon below has TWO bugs between it and a clean shutdown: its
// stop flag is a type no handler may touch, and its handler is
// subscribed to the wrong channel.
//
// Task: make the program compile and run to completion with every
// assert passing.
//   - the static_assert pins the flag's exact type
//   - raise(SIGUSR1) must run YOUR handler; the loop must then exit and
//     main return 0
//   - once it compiles but before the second fix, the watcher will show
//     the process KILLED by SIGUSR1 (an unhandled signal's default
//     action) rather than an assert — that's the second bug talking
// Constraints:
//   - the handler body stays a single flag assignment (see the safe
//     list above)
//   - don't change the asserts, the static_assert, or the raise line

#include <cassert>
#include <csignal>       // std::signal, std::raise, std::sig_atomic_t
#include <type_traits>

// The shutdown flag. A handler is allowed to touch this one... right?
bool g_stop = false;

static_assert(std::is_same_v<decltype(g_stop), volatile std::sig_atomic_t>,
              "a signal handler may only write a volatile "
              "std::sig_atomic_t (or a lock-free std::atomic)");

// Handlers are called by the C runtime, so give them C linkage.
extern "C" void on_shutdown(int) {
    g_stop = 1;    // set the flag, return — nothing else is safe here
}

int main() {
    // Subscribe to the shutdown request. (std::signal is the portable
    // minimum; serious POSIX code uses sigaction for the extra control.)
    std::signal(SIGINT, on_shutdown);

    long iterations = 0;
    while (!g_stop) {
        ++iterations;               // ...the daemon's actual work...
        if (iterations == 10'000) {
            std::raise(SIGUSR1);    // the "please shut down" request lands
        }
        if (iterations > 10'000) {
            break;                  // watchdog: a missed flag must not
        }                           // hang the watcher forever
    }

    assert(g_stop == 1);            // the handler ran and the loop SAW it
    assert(iterations == 10'000);   // exit was immediate, not a lap later
    return 0;
}

// I AM NOT DONE
