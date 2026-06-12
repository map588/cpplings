// ub5.cpp
//
// Integer division by zero is UB. Not an exception, not infinity, not
// zero — undefined. (FLOATING-POINT division is the opposite: IEEE 754
// defines x/0.0 as ±inf and 0.0/0.0 as NaN, no UB at all. The two
// rulebooks differ and people mix them up constantly.)
//
// And its obscure sibling, same UBSan check: INT_MIN / -1 — the one
// division whose true result (+2147483648) doesn't fit in int.
// Historically a process-killer (x86 raises SIGFPE); famously usable
// as a denial-of-service against careless servers.
//
// The specimen is the most ordinary code imaginable: an average over a
// filter that can match nothing. Empty set → count == 0 → boom. Every
// "mean", "rate", "percentage" function ever written has had this bug
// at some point.
//
// Task: handle the empty case explicitly (return 0 here — or an
// optional, module 19, in a design of your own).

// I AM NOT DONE

#include <cassert>
#include <vector>

// Average of the readings above the threshold.
int average_above(const std::vector<int>& readings, int threshold) {
    int sum = 0;
    int count = 0;
    for (int r : readings) {
        if (r > threshold) {
            sum += r;
            ++count;
        }
    }
    return sum / count;
}

int main() {
    std::vector<int> temps = {18, 31, 24, 35};

    assert(average_above(temps, 30) == 33);    // (31+35)/2
    assert(average_above(temps, 100) == 0);    // nothing qualifies → /0
    return 0;
}
