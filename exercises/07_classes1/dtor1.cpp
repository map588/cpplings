// dtor1.cpp
//
// Destruction order is fully specified, and it's always the REVERSE of
// construction:
//
//   - locals in a scope:  last constructed, first destroyed
//   - members of a class: reverse of DECLARATION order
//   - base class:         after the derived class's members
//
// This symmetry is what makes RAII compose: if resource B was acquired
// using resource A, B was constructed after A, so B is released before
// A — dependencies unwind in exactly the right order, even when an
// exception is flying (module 20).
//
// The probes below append to a log: their tag on construction, '~' + tag
// on destruction. Predict the log strings. (Note Pair: its members were
// declared `right` THEN `left` — declaration order is what counts.)
//
// Task: fill in the two TODO strings.

#include <cassert>
#include <string>

std::string TODO = "?";

struct Probe {
    char tag;
    std::string& log;
    Probe(char t, std::string& l) : tag(t), log(l) { log += tag; }
    ~Probe() { log += '~'; log += tag; }
};

struct Pair {
    Probe right;   // declared first → constructed first → destroyed LAST
    Probe left;
    Pair(std::string& l) : right('r', l), left('l', l) {}
};

int main() {
    {
        std::string log;
        {
            Probe a('a', log);
            Probe b('b', log);
        }
        assert(log == TODO);
    }
    {
        std::string log;
        {
            Pair p(log);
        }
        assert(log == TODO);
    }
    return 0;
}

// I AM NOT DONE
