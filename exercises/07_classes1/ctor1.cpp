// ctor1.cpp
//
// There are two places a constructor can give members values:
//
//   Sensor(int id) : id_(id) { }     // MEMBER INIT LIST: initialization
//   Sensor(int id) { id_ = id; }     // body: ASSIGNMENT after the fact
//
// By the time the body runs, every member has ALREADY been constructed
// (default-constructed, if the init list didn't say otherwise). So the
// body version pays twice for class-type members — default-construct,
// then assign — and for two kinds of members it can't work at all:
//
//   const members      — can't assign to const
//   reference members  — can't reseat a reference (module 03!)
//
// One more rule that bites: members initialize in DECLARATION order, not
// init-list order. If your list says `: b_(x), a_(b_)` but a_ is declared
// first, a_ reads an uninitialized b_. (-Wreorder warns; this repo's
// flags include it via -Wall.)
//
// Task: convert this constructor to a member init list.

// I AM NOT DONE

#include <cassert>
#include <string>

class Sensor {
public:
    Sensor(int id, const std::string& name) {
        id_ = id;           // const member: assignment won't compile
        name_ = name;       // works, but constructs empty then assigns
        reading_ = 0;
    }

    int id() const { return id_; }
    const std::string& name() const { return name_; }
    int reading() const { return reading_; }

private:
    const int id_;          // identity never changes — rightly const
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
