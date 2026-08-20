// hints1.cpp — SOLUTION
//
// The promise becomes a return value: -1 for "not a digit". The
// [[likely]]/[[unlikely]] hints stay — they describe real input
// statistics and cost nothing if wrong. __builtin_unreachable stays
// in the toolbox for paths that are PROVABLY dead (a switch over a
// checked enum), never for input you don't control.

#include <cassert>

// Digits dominate real input, so the happy path carries the hint.
int parse_digit(char c) {
    if (c >= '0' && c <= '9') [[likely]] {
        return c - '0';
    }
    return -1;                 // total function: junk has an answer too
}

int main() {
    // Trusted input: fine either way.
    assert(parse_digit('7') == 7);
    assert(parse_digit('0') == 0);

    // Untrusted input: sum the digits, skip the junk.
    const char* input = "4x7q2";
    int sum = 0;
    for (const char* p = input; *p != '\0'; ++p) {
        int d = parse_digit(*p);
        if (d < 0) [[unlikely]] {
            continue;                     // junk is the rare case
        }
        sum += d;
    }
    assert(sum == 13);
    return 0;
}
