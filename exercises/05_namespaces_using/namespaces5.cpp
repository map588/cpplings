// namespaces5.cpp
//
// Scoped enums — `enum class` (C++11) — fixed two real problems with the
// old `enum`:
//
//   enum Color { Red };          // old: Red leaks into the enclosing
//                                // scope, and Color converts to int
//                                // silently (Red < 3.14 compiles!)
//   enum class Color { Red };    // new: spell it Color::Red, and NO
//                                // implicit conversion to int
//
// The cost was verbosity — switches got noisy:
//
//   case Color::Red: ... case Color::Green: ... case Color::Blue: ...
//
// C++20's `using enum` (the fourth face of `using`) gives the brevity
// back, scoped to wherever you put it — typically right inside the switch:
//
//   switch (c) {
//       using enum Color;
//       case Red: ...
//   }
//
// Task: the switch uses bare enumerator names without bringing them into
// scope. One line fixes it. (Qualifying all four cases also works — do
// the C++20 way.)

// I AM NOT DONE

#include <cassert>
#include <type_traits>

enum class Signal { Red, Yellow, Green, Off };

// enum class really doesn't convert: (good!)
static_assert(!std::is_convertible_v<Signal, int>);

const char* describe(Signal s) {
    switch (s) {
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
