// default1.cpp — solution

#include <cassert>
#include <string>

struct Session {
    std::string user = "guest";
    int timeout_ms = 1000;

    Session() = default;                 // back, powered by the NSDMIs
    Session(const std::string& u) : user(u) {}
};

int g_timeout_ms = 0;

void set_timeout_ms(int ms) {
    g_timeout_ms = ms;
}
void set_timeout_ms(double) = delete;    // fractional milliseconds: refused

int main() {
    Session anonymous;
    assert(anonymous.user == "guest");

    Session alice("alice");
    assert(alice.user == "alice" && alice.timeout_ms == 1000);

    set_timeout_ms(2500);                // now the caller must say it in ms
    assert(g_timeout_ms == 2500);
    return 0;
}
