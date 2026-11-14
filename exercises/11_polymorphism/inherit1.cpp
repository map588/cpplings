// inherit1.cpp
//
// Two things a derived class does NOT get from its base, although
// everyone assumes it does.
//
// CONSTRUCTORS ARE NOT INHERITED. A derived class that declares none
// gets only the compiler-generated ones (default, copy, move). The
// base's `Base(std::string, int)` is not a way to build a Derived.
//
// A NAME IN THE DERIVED CLASS HIDES THAT NAME IN THE BASE — every
// overload of it, whatever the parameters. Name lookup walks outward
// scope by scope and STOPS at the first scope that has the name;
// overload resolution then only sees what lookup found. So adding one
// overload in the derived class does not JOIN the base's overload set.
// It REPLACES it. And if an argument can convert to the one visible
// overload, there is no error: the call quietly lands on the wrong
// function. (This is hiding, not overriding — override1's disease in a
// non-virtual body. No `virtual` is involved at all.)
//
// The cure for both is a using-declaration — the fifth face of `using`
// that module 05 promised. Its shape, on somebody else's classes:
//
//     struct Circle : Shape {
//         using Shape::Shape;    // inherit Shape's constructors (C++11)
//         using Shape::draw;     // bring EVERY Shape::draw overload
//                                // into this scope, next to mine
//     };
//
// GaugeLog below wanted to ADD a way to record a measurement. As
// written, it cannot be constructed the way main does — and once you
// fix that, watch which record() the int and the char calls reach.
//
// Task: make GaugeLog a proper extension of EventLog.
//   - main constructs GaugeLog with EventLog's constructors, as written
//   - each record() call reaches the overload that matches its argument
//     EXACTLY — the returned letter says which one ran
// Constraints:
//   - no hand-written constructors and no forwarding functions in
//     GaugeLog: two using-declarations do the whole job
//   - GaugeLog keeps its record(double); EventLog and main stay as
//     they are
//   - fix the constructor problem first and RUN before fixing the
//     second one — seeing the silent wrong call is the lesson

#include <cassert>
#include <string>
#include <utility>

class EventLog {
public:
    explicit EventLog(std::string name) : name_(std::move(name)) {}
    EventLog(std::string name, int level)
        : name_(std::move(name)), level_(level) {}

    // Each overload returns a letter, so the asserts can see who ran.
    char record(int)  { return 'i'; }    // an event count
    char record(char) { return 'c'; }    // a one-letter status flag

    const std::string& name() const { return name_; }
    int level() const { return level_; }

private:
    std::string name_;
    int level_ = 1;
};

class GaugeLog : public EventLog {
public:
    // "Just one more overload", said the author.
    char record(double) { return 'd'; }  // a measurement
};

int main() {
    GaugeLog net("net", 3);              // EventLog's two-argument constructor
    assert(net.name() == "net" && net.level() == 3);

    GaugeLog disk("disk");               // ...and its one-argument one
    assert(disk.level() == 1);

    assert(net.record(2.5) == 'd');      // the new overload
    assert(net.record(42) == 'i');       // an int: which record() is this?
    assert(net.record('x') == 'c');      // a char: and this?
    return 0;
}

// I AM NOT DONE
