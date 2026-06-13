// conc3.cpp        (ThreadSanitizer build)
//
// Two threads push_back into one vector. Everything about that is a
// race: the size bookkeeping, the element writes, the occasional
// REALLOCATION moving the buffer while the other thread writes into
// it (module 16's invalidation, now with two actors). TSan reports it;
// without TSan you'd see rare crashes and corrupted logs in the field.
//
// std::atomic can't help — a vector isn't a single atomic object.
// This is mutex territory:
//
//     std::mutex m;                  // the lock
//     {
//         std::lock_guard lk(m);     // RAII: locks now, unlocks at }
//         shared.push_back(x);       // ...even on exception (module 20)
//     }
//
// Keep the locked region SHORT — format the string before the lock,
// append under it. (lock_guard is the C++11 classic; scoped_lock
// (C++17) is its multi-mutex successor — conc4 shows why that
// matters.)
//
// Task: add the mutex and guard the push_back.

#include <cassert>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

std::vector<std::string> event_log;

void record_events(const char* who) {
    for (int i = 0; i < 1'000; ++i) {
        std::string entry = std::string(who) + "#" + std::to_string(i);
        event_log.push_back(std::move(entry));     // two writers, no lock
    }
}

int main() {
    {
        std::jthread a(record_events, "net");
        std::jthread b(record_events, "disk");
    }

    assert(event_log.size() == 2'000);
    return 0;
}

// I AM NOT DONE
