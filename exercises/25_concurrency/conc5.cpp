// conc5.cpp        (ThreadSanitizer build)
//
// std::condition_variable: sleep until someone signals a condition.
// The machinery is a three-piece set — mutex, cv, and A PREDICATE
// (the actual condition, as data):
//
//     std::unique_lock lk(m);              // cv needs unique_lock
//     cv.wait(lk, [] { return ready; });   // sleeps; rechecks `ready`
//                                          // (under the lock) on wake
//
// The shipped code uses the predicate-LESS wait — and demonstrates the
// LOST WAKEUP: the producer finishes fast and calls notify_one BEFORE
// the consumer reaches wait(). A notify with nobody waiting vanishes;
// the consumer then sleeps forever (the runner's timeout will report
// it). The other killer the bare wait ignores: SPURIOUS WAKEUPS —
// the OS may wake waiters with no notify at all; without a predicate
// recheck you proceed on a condition that isn't true.
//
// The predicate overload survives both: checks before sleeping (lost
// wakeup → no sleep at all), rechecks on every wake (spurious → back
// to sleep). THE RULE: never call wait() without a predicate.
//
// Task: one argument to add.

// I AM NOT DONE

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
