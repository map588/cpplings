// templates4.cpp
//
// Sometimes one type needs a DIFFERENT implementation than the generic
// recipe. That's (full) SPECIALIZATION:
//
//     template <typename T>             // the generic recipe
//     struct Formatter { ... };
//
//     template <>                       // "for bool, use THIS instead"
//     struct Formatter<bool> {
//         static std::string format(bool b) { ... }
//     };
//
// The empty `template <>` says "nothing left to deduce — this is the
// concrete recipe for exactly Formatter<bool>." A specialization
// replaces the generic class WHOLESALE: it shares nothing, inherits
// nothing, must declare every member it wants.
//
// (For FUNCTION templates, prefer plain overloads over specialization —
// overload resolution considers overloads but largely ignores function
// specializations; that asymmetry is a classic interview gotcha. Class
// templates: specialization is the normal tool, and the standard library
// is full of it — std::hash<YourType>, std::formatter<T>, char_traits.)
//
// Task: bools are printing as "1". Specialize Formatter for bool to say
// "yes"/"no".

// I AM NOT DONE

#include <cassert>
#include <string>

template <typename T>
struct Formatter {
    static std::string format(const T& value) {
        return std::to_string(value);
    }
};

int main() {
    assert(Formatter<int>::format(42) == "42");
    assert(Formatter<double>::format(2.5).substr(0, 3) == "2.5");

    assert(Formatter<bool>::format(true) == "yes");    // "1" today
    assert(Formatter<bool>::format(false) == "no");    // "0" today
    return 0;
}
