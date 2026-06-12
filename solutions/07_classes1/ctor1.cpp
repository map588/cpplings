// ctor1.cpp — solution

#include <cassert>
#include <string>

class Sensor {
public:
    // Initialization, not construct-then-assign. List order matches
    // declaration order (id_, name_, reading_).
    Sensor(int id, const std::string& name)
        : id_(id), name_(name), reading_(0) {}

    int id() const { return id_; }
    const std::string& name() const { return name_; }
    int reading() const { return reading_; }

private:
    const int id_;
    std::string name_;
    int reading_;
};

int main() {
    Sensor s(42, "thermocouple");
    assert(s.id() == 42);
    assert(s.name() == "thermocouple");
    assert(s.reading() == 0);
    return 0;
}
