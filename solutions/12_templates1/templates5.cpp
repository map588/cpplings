// templates5.cpp — solution

#include <cassert>
#include <sstream>
#include <string>
#include <vector>

template <typename T>
std::string join_lines(const std::vector<T>& items) {
    std::ostringstream out;
    for (const T& item : items) {
        out << item << '\n';
    }
    return out.str();
}

struct LogEntry {
    std::string level;
    std::string message;

    // Hidden friend (module 08): found by ADL when a LogEntry meets <<.
    friend std::ostream& operator<<(std::ostream& os, const LogEntry& e) {
        return os << e.level << ": " << e.message;
    }
};

int main() {
    std::vector<LogEntry> log = {
        {"INFO", "started"},
        {"WARN", "disk slow"},
    };

    assert(join_lines(log) == "INFO: started\nWARN: disk slow\n");

    std::vector<int> nums = {1, 2};
    assert(join_lines(nums) == "1\n2\n");
    return 0;
}
