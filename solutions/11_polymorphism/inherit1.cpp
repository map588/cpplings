// inherit1.cpp — solution

#include <cassert>
#include <string>
#include <utility>

class EventLog {
public:
    explicit EventLog(std::string name) : name_(std::move(name)) {}
    EventLog(std::string name, int level)
        : name_(std::move(name)), level_(level) {}

    char record(int)  { return 'i'; }
    char record(char) { return 'c'; }

    const std::string& name() const { return name_; }
    int level() const { return level_; }

private:
    std::string name_;
    int level_ = 1;
};

class GaugeLog : public EventLog {
public:
    using EventLog::EventLog;   // inheriting constructors (C++11)
    using EventLog::record;     // un-hide record(int) and record(char):
                                // all three now compete in one overload set
    char record(double) { return 'd'; }
};

int main() {
    GaugeLog net("net", 3);
    assert(net.name() == "net" && net.level() == 3);

    GaugeLog disk("disk");
    assert(disk.level() == 1);

    assert(net.record(2.5) == 'd');
    assert(net.record(42) == 'i');       // shipped: 'd' — int → double, silently
    assert(net.record('x') == 'c');      // shipped: 'd' too
    return 0;
}
