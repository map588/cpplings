// default1.cpp
//
// `= default` and `= delete` (both C++11): asking for, and refusing,
// compiler-generated behavior.
//
//   RULE: the moment you write ANY constructor, the compiler stops
//   generating the default constructor. The Session class below gained a
//   Session(int) and silently lost `Session s;`. Ask for it back:
//
//       Session() = default;        // with NSDMIs supplying the values
//
//   `= delete` is the opposite tool: remove a function from overload
//   resolution. It works on ANY function — and a deleted OVERLOAD is the
//   standard trick for blocking an unwanted implicit conversion:
//
//       void set_timeout_ms(int ms);
//       void set_timeout_ms(double) = delete;   // "no, you're thinking
//                                               //  of seconds"
//
//   Without it, set_timeout_ms(2.5) silently truncates to 2ms (module 01
//   all over again — but now at an API boundary you own).
//
// Task: restore Session's default constructor, then delete the double
// overload and fix the buggy call.

// I AM NOT DONE

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
