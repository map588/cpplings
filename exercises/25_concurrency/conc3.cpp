// conc3.cpp        (ThreadSanitizer build)
//
// Two threads push_back into one vector. Everything about that is a
// race: the size bookkeeping, the element writes, the occasional
// REALLOCATION moving the buffer while the other thread writes into
// it (module 16's invalidation, now with two actors). TSan reports it;
// without TSan you'd see rare crashes and corrupted logs in the field.
//
// std::atomic can't help — a vector isn't a single atomic object.
// This is mutex territory: one lock owned RAII-style by a guard
// object, held for exactly the region that touches the shared thing —
// acquired at the guard's birth, released at its death, even on
// exception (module 20's discipline, applied to locks).
//
// Keep the locked region SHORT: threads that serialize their string-
// building had no reason to be threads. Build outside, append under
// the lock.
//
// Task: make the concurrent logging safe.
//   - the assert passes: all 2'000 entries arrive
//   - runs clean under TSan (no data-race report)
// Constraints:
//   - the string construction stays OUTSIDE the locked region
//   - both threads keep running concurrently — don't join one before
//     starting the other
//   - keep the loop counts and the assert

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
