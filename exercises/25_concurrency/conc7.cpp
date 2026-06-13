// conc7.cpp
//
// Task-level parallelism without touching std::thread:
//
//     std::future<int> f = std::async(std::launch::async, work, args);
//     ...                                  // work runs concurrently
//     int result = f.get();                // wait + retrieve (or rethrow
//                                          // the task's exception!)
//
// std::async returns a FUTURE — a one-shot channel for the result.
// Three things worth tattooing somewhere:
//
//   - pass std::launch::async explicitly. The default policy MAY defer
//     the task to run lazily on .get() — your "parallel" code, serial.
//   - .get() once only; it moves the result out (valid() goes false).
//   - the future from std::async BLOCKS IN ITS DESTRUCTOR until the
//     task finishes (infamous; it means an unused future is a sync
//     point, not a fire-and-forget).
//
// Exceptions travel too: a task that throws stores the exception, and
// .get() rethrows it in the calling thread — error handling across
// threads for free (module 20 wired straight through).
//
// Task: parallel_sum runs its halves sequentially. Run the first half
// as an async task while this thread does the second; combine with
// .get(). Note there's NO shared mutable state — each task owns its
// range. That's why no mutex appears: structure, not locks.

#include <cassert>
#include <future>
#include <numeric>
#include <vector>

long range_sum(const std::vector<int>& v, std::size_t lo, std::size_t hi) {
    long total = 0;
    for (std::size_t i = lo; i < hi; ++i) total += v[i];
    return total;
}

long parallel_sum(const std::vector<int>& v) {
    std::size_t mid = v.size() / 2;

    long front = range_sum(v, 0, mid);        // TODO: as an async task
    long back = range_sum(v, mid, v.size());  // ...while we do this half
    return front + back;
}

int main() {
    std::vector<int> data(10'000);
    for (std::size_t i = 0; i < data.size(); ++i) {
        data[i] = static_cast<int>(i % 7);
    }

    long expected = std::accumulate(data.begin(), data.end(), 0L);
    assert(parallel_sum(data) == expected);

    // Prove you know the future API — what does the SECOND get() do?
    auto f = std::async(std::launch::async, [] { return 5; });
    assert(f.get() == 5);
    assert(f.valid() == true);   // TODO: true or false after a get()?
    return 0;
}

// I AM NOT DONE
