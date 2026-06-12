// namespaces5.cpp — solution

#include <cassert>
#include <type_traits>

enum class Signal { Red, Yellow, Green, Off };

static_assert(!std::is_convertible_v<Signal, int>);

const char* describe(Signal s) {
    switch (s) {
        using enum Signal;   // C++20: enumerators in scope, only in here
        case Red:    return "stop";
        case Yellow: return "hurry";
        case Green:  return "go";
        case Off:    return "look both ways";
    }
    return "?";
}

int main() {
    assert(describe(Signal::Green)[0] == 'g');
    assert(describe(Signal::Off)[0] == 'l');
    return 0;
}
