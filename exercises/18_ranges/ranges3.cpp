// ranges3.cpp
//
// Laziness unlocks something loops can't express: ranges with NO END.
//
//     std::views::iota(1)        // 1, 2, 3, ... forever
//
// An infinite sequence is fine as long as something downstream bounds
// CONSUMPTION:
//
//     auto first_squares = std::views::iota(1)
//                        | std::views::transform([](int n) { return n*n; })
//                        | std::views::take(5);    // 1 4 9 16 25, done
//
// The pipeline never "runs ahead" — take(5) pulls five elements through
// and stops pulling. Also in the toolbox: views::drop(n) (skip the
// first n), and iota(a, b) for finite [a, b).
//
// Task: implement perfect_squares_ending_in_6(k) — the first k perfect
// squares whose last digit is 6 (4²=16, 6²=36, 14²=196, ...) — as ONE
// view pipeline over an infinite iota. The skeleton consumes the
// pipeline for you.
//   - both asserts pass, and the program terminates (an unbounded
//     pipeline never would — something must limit consumption)
// Constraints:
//   - keep the consuming for-loop as it is; the pipeline's stages
//     express the whole computation — no handwritten square/digit loop
//   - think about the ORDER of your stages: what value does each stage
//     actually see?

#include <cassert>
#include <ranges>
#include <vector>

std::vector<int> perfect_squares_ending_in_6(int k) {
    // Currently an EMPTY range (iota(1, 1) is [1, 1) — nothing) so the
    // skeleton terminates. Swap in the infinite iota(1) — but only once
    // something downstream bounds consumption, or the loop below never
    // ends!
    auto pipeline = std::views::iota(1, 1);   // TODO: build the real pipeline
    std::vector<int> out;
    for (int x : pipeline) out.push_back(x);
    return out;
}

int main() {
    assert(perfect_squares_ending_in_6(1) == (std::vector<int>{16}));
    assert(perfect_squares_ending_in_6(4)
           == (std::vector<int>{16, 36, 196, 256}));
    return 0;
}

// I AM NOT DONE
