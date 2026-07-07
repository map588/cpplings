// extras1.cpp
//
// `int timeout` — of WHAT? Every API that answers "milliseconds, it's
// in the docs" has a 3 AM incident in its future. <chrono> (C++11)
// puts the unit in the TYPE:
//
//     std::chrono::milliseconds timeout{250};
//     using namespace std::chrono_literals;       // C++14
//     auto t = 250ms;  auto limit = 2h;           // literals
//
// Conversion rules carry the safety:
//   - LOSSLESS conversions are implicit: seconds → milliseconds, fine.
//   - LOSSY ones don't compile: milliseconds → seconds needs an
//     explicit duration_cast (truncation you must sign for).
//   - .count() unwraps the raw number — keep it at the very edges.
//
// retry_delay() below mixes bare ints and hope — its "cap at 3"
// compares a milliseconds-flavored 100 against a seconds-flavored 3
// and calls it policy. The asserts speak chrono: they compare the
// result against 100ms and against 3s directly, so both the doubling
// and the cap must mean what they say.
//
// Task: give retry_delay chrono types end to end.
//   - returns the unit-safe "100ms doubled per attempt" (100, 200,
//     400, ...), capped at three actual seconds
//   - all asserts in main pass, unchanged
// Constraints:
//   - the return type is a chrono duration — no bare int in the
//     signature
//   - no .count() inside retry_delay: let the TYPES do the comparing
//   - don't change main

#include <cassert>
#include <chrono>

using namespace std::chrono_literals;

int retry_delay(int attempt) {       // returns... units of optimism?
    int delay = 100 * (1 << attempt);
    if (delay > 3) delay = 3;        // cap at 3 (3 WHAT? exactly.)
    return delay;
}

int main() {
    assert(retry_delay(0) == 100ms);
    assert(retry_delay(1) == 200ms);
    assert(retry_delay(3) == 800ms);

    assert(retry_delay(10) == 3s);             // capped — and note: the
                                               // comparison converts 3s
                                               // to ms losslessly, free
    // Lossy direction needs a signature:
    auto precise = 2500ms;
    auto coarse = std::chrono::duration_cast<std::chrono::seconds>(precise);
    assert(coarse == 2s);                      // truncated, explicitly
    assert(coarse.count() == 2);               // raw number, at the edge
    return 0;
}

// I AM NOT DONE
