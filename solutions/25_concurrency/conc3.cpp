// conc3.cpp — solution

#include <cassert>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

std::vector<std::string> event_log;
std::mutex log_mutex;

void record_events(const char* who) {
    for (int i = 0; i < 1'000; ++i) {
        // Build OUTSIDE the lock — hold it only for the append:
        std::string entry = std::string(who) + "#" + std::to_string(i);
        {
            std::lock_guard lk(log_mutex);
            event_log.push_back(std::move(entry));
        }
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
