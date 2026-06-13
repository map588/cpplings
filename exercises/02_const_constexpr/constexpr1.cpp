// constexpr1.cpp
//
// `const` does NOT mean compile-time. It means "won't change after
// initialization" — and the initialization can happen at runtime:
//
//     const int x = read_from_config();   // perfectly legal
//
// `constexpr` on a variable (C++11) is the stronger claim: "this value is
// computed AT COMPILE TIME." Only constexpr-evaluable values can be used
// where the language demands constants: array bounds, template arguments,
// static_assert, case labels.
//
// For a FUNCTION to be callable at compile time it must itself be declared
// constexpr (C++11; since C++14 the body can use loops, ifs, and local
// variables like normal code).
//
// Task: max_packet_size() is a pure computation, but it isn't usable at
// compile time, so neither the constexpr variable nor the array bound
// compiles. One keyword fixes everything.

#include <cassert>
#include <cstddef>

int max_packet_size(int header_bytes, int payload_bytes) {
    int total = header_bytes + payload_bytes;
    // round up to a multiple of 8
    if (total % 8 != 0) {
        total += 8 - total % 8;
    }
    return total;
}

int main() {
    constexpr int packet_size = max_packet_size(11, 32);
    static_assert(packet_size == 48, "11+32=43, rounded up to 48");

    // Array bounds must be compile-time constants (this is a real C++
    // array — unlike C, C++ has no variable-length arrays).
    unsigned char buffer[max_packet_size(11, 32)] = {};
    assert(sizeof(buffer) == 48);

    // And the SAME function still works on runtime values:
    int runtime_header = 20;
    assert(max_packet_size(runtime_header, 100) == 120);
    return 0;
}

// I AM NOT DONE
