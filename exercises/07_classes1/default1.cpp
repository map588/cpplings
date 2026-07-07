// default1.cpp
//
// `= default` and `= delete` (both C++11): asking for, and refusing,
// compiler-generated behavior.
//
//   RULE: the moment you write ANY constructor, the compiler stops
//   generating the default constructor. The Session class below gained a
//   Session(const std::string&) and silently lost `Session s;`. There's
//   nothing to hand-write back: the NSDMIs already describe a default
//   Session — you only need to ask the compiler to generate the
//   constructor again.
//
//   `= delete` is the opposite tool: remove a function from overload
//   resolution. It works on ANY function, not just special members —
//   and deleting a well-chosen OVERLOAD is the standard trick for
//   turning an unwanted implicit conversion into a compile error.
//   Right now set_timeout_ms(2.5) silently truncates to 2ms (module 01
//   all over again — but now at an API boundary you own).
//
// Task: restore `Session s;`, and make fractional-millisecond calls
// impossible.
//   - the program compiles and every assert passes
//   - set_timeout_ms(2.5) must become a COMPILE ERROR — verify that,
//     then fix the call to say what it means in integer milliseconds
// Constraints:
//   - keep Session(const std::string&) and both NSDMIs
//   - no hand-written body for the default constructor
//   - set_timeout_ms(int) keeps its signature; don't change the asserts

#include <cassert>
#include <string>

struct Session {
    std::string user = "guest";
    int timeout_ms = 1000;

    Session(const std::string& u) : user(u) {}
};

int g_timeout_ms = 0;

void set_timeout_ms(int ms) {
    g_timeout_ms = ms;
}

int main() {
    Session anonymous;                   // lost when Session(string) arrived
    assert(anonymous.user == "guest");

    Session alice("alice");
    assert(alice.user == "alice" && alice.timeout_ms == 1000);

    set_timeout_ms(2.5);                 // 2.5 WHAT? truncates to 2ms.
    assert(g_timeout_ms == 2500);        // caller clearly meant seconds
    return 0;
}

// I AM NOT DONE
