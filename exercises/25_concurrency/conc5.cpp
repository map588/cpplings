// conc5.cpp        (ThreadSanitizer build)
//
// std::condition_variable: sleep until someone signals a condition.
// The machinery is a three-piece set — mutex, cv, and A PREDICATE
// (the actual condition, as data):
//
//     std::unique_lock lk(m);          // cv needs unique_lock
//     cv.wait(lk, /* ...and what? */); // the third piece goes here
//
// The shipped code uses the predicate-LESS wait — and demonstrates the
// LOST WAKEUP: the producer finishes fast and calls notify_one BEFORE
// the consumer reaches wait(). A notify with nobody waiting vanishes;
// the consumer then sleeps forever (the runner's timeout will report
// it). The other killer the bare wait ignores: SPURIOUS WAKEUPS —
// the OS may wake waiters with no notify at all; without rechecking
// the actual condition you proceed on a state that isn't true.
//
// A wait that survives both must (a) check the condition BEFORE
// sleeping — a notify that already happened means there is nothing
// left to wait for — and (b) RE-check it on every wake. The wait API
// can do both for you if you hand it the condition itself; `ready` is
// already sitting there as data. THE RULE: never wait on a cv without
// stating what you're waiting FOR.
//
// Task: make the consumer immune to the lost wakeup.
//   - the program finishes (no timeout) and the assert sees "42"
//   - runs clean under TSan
// Constraints:
//   - keep the consumer's 100ms sleep — arriving late IS the test
//   - keep the cv machinery: no busy-waiting on the flag, no removing
//     notify_one, `ready` remains the condition

#include <cassert>
#include <chrono>
#include <condition_variable>
#include <mutex>
#include <string>
#include <thread>

std::mutex m;
std::condition_variable cv;
std::string result;
bool ready = false;

void producer() {
    {
        std::lock_guard lk(m);
        result = "42";
        ready = true;
    }
    cv.notify_one();             // fires immediately — consumer not there yet
}

void consumer(std::string& out) {
    // Arrive fashionably late, well after the notify:
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    std::unique_lock lk(m);
    cv.wait(lk);                 // the notify is long gone. Sleep forever.
    out = result;
}

int main() {
    std::string answer;
    {
        std::jthread p(producer);
        std::jthread c(consumer, std::ref(answer));
    }
    assert(answer == "42");
    return 0;
}

// I AM NOT DONE
