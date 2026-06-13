// ub3.cpp
//
// An object dies at its closing brace — the LANGUAGE says so — but the
// STACK MEMORY underneath usually lingers, still holding plausible
// bytes. That gap is where use-after-scope bugs hide: code that reads a
// dead object and "works" for years, until inlining or a new local
// reuses the slot.
//
//     const int* p;
//     {
//         int candidate = best_so_far();
//         p = &candidate;            // points at a doomed local
//     }                              // candidate is DEAD here
//     use(*p);                       // UB — plausible bytes, no object
//
// ASan instruments this exact pattern (stack-use-after-scope): scoped-
// out frames get poisoned, and the read below lights up with the name
// of the dead variable.
//
// The shape below is the realistic one: a "remember the best element"
// loop that remembers an ADDRESS inside the loop body — every iteration's
// `score` is a fresh object that dies at the iteration's end.
//
// Task: track the best VALUE (or an index into `entries`), not the
// address of a per-iteration local.

#include <cassert>
#include <vector>

int main() {
    std::vector<int> entries = {12, 47, 8, 31};

    const int* best = nullptr;
    for (std::size_t i = 0; i < entries.size(); ++i) {
        int score = entries[i] * 2;        // born each iteration...
        if (best == nullptr || score > *best) {
            best = &score;                 // ...remembered by address...
        }
    }                                      // ...dead since long ago

    assert(*best == 94);                   // 47 * 2, read from a ghost
    return 0;
}

// I AM NOT DONE
