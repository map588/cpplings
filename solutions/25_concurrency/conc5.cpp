// conc5.cpp — solution

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
    cv.notify_one();
}

void consumer(std::string& out) {
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    std::unique_lock lk(m);
    cv.wait(lk, [] { return ready; });   // checks first: ready is already
    out = result;                        // true → no sleep, no lost wakeup
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
