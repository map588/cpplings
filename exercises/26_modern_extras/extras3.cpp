// extras3.cpp
//
// Thirty years of C++ string formatting:
//
//   sprintf(buf, "%s: %d", name, n)    // fast; type-UNSAFE (%d vs long:
//                                      // UB), buffer overflows included
//   oss << name << ": " << n           // safe; verbose, stateful
//                                      // manipulators, slow
//   std::format("{}: {}", name, n)     // C++20: safe, fast, readable —
//                                      // returns std::string
//
// The mini-language in the braces ({index:fill/align/width/.precision}):
//
//     {}          next argument, natural form
//     {:.1f}      fixed, 1 decimal
//     {:>8}       right-aligned in 8 columns      {:<8} left
//     {:08.3f}    zero-padded width 8, 3 decimals
//     {:x}        hex  (# adds the 0x prefix; combine with 0-padding
//                 and a width for fixed-size dumps)
//     {0} {1} {0} positional — reuse arguments
//
// Format-string typos (wrong spec for the type, too few args) are
// COMPILE ERRORS — the format string is parsed at compile time.
// (C++23: std::print("...\n", args) writes straight to stdout.)
//
// Task: implement describe_transfer, then settle the warm-ups.
//   - both describe_transfer asserts pass — read their expected
//     strings closely; the spacing before the amount is not an
//     accident
//   - the three warm-up TODOs replaced by what std::format actually
//     produces
// Constraints:
//   - describe_transfer is ONE std::format call: no manual padding,
//     no string concatenation
//   - warm-ups: change only the expected "TODO" strings, never the
//     format calls

#include <cassert>
#include <format>
#include <string>

// "alice -> bob:    42.50 MB (7%)"   — names, amount, percent; count
// the columns the amount occupies in both asserts.
std::string describe_transfer(const std::string& from, const std::string& to,
                              double mb, int percent) {
    return "";   // TODO: one std::format call
}

int main() {
    assert(describe_transfer("alice", "bob", 42.5, 7)
           == "alice -> bob:    42.50 MB (7%)");
    assert(describe_transfer("db", "tape", 1337.126, 99)
           == "db -> tape:  1337.13 MB (99%)");

    // Warm-ups (fix the expected strings instead of the format calls):
    assert(std::format("{:>6}", 42) == "TODO");
    assert(std::format("{:#06x}", 42) == "TODO");
    assert(std::format("{0}{1}{0}", "|", "core") == "TODO");
    return 0;
}

// I AM NOT DONE
