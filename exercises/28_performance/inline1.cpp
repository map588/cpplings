// inline1.cpp
//
// `inline` is the most mis-taught keyword in C++. What people think
// it means: "make this function fast / paste it at the call site."
// What it ACTUALLY means since forever: "this definition appears in
// several translation units ON PURPOSE — linker, fold them into one."
// It is an ODR permission slip (module 06), not a speed knob.
//
// The two meanings parted ways decades ago:
//
//   - INLINING (the optimization) is the optimizer's call. At -O2 it
//     inlines any function it can see and judges worth it — keyword
//     or no keyword — and ignores your `inline` when it disagrees.
//   - `inline` (the keyword) changes LINKAGE rules. Its real modern
//     job: let definitions live in headers. Same story for
//     variables: inline variables (C++17), module 06.
//
// When you genuinely must overrule the optimizer's judgment — hot
// interpreter loops, or keeping a cold error path from bloating a
// caller — the tools are attributes, not `inline`:
//
//     [[gnu::always_inline]] inline int hot(...)  { ... }
//     [[gnu::noinline]]      int cold_report(...) { ... }
//
// (gnu:: works in clang and gcc both. MSVC spells them
// __forceinline / __declspec(noinline). Measure first!)
//
// This exercise is the linkage half, live: inline1.h defines mean()
// in the header — the right place for it, every TU can see and
// inline it. But compiled as-is, inline1.cpp and inline1_stats.cpp
// EACH emit a strong definition of mean(), and the LINKER — not the
// compiler — rejects the build with "duplicate symbol". Read the
// error: it names the symbol and both object files.
//
// Task: one keyword in inline1.h makes the two definitions legal —
// and identical-by-construction, since both come from the same
// header text.
// Constraints:
//   - mean() stays defined in the header (that's the point)
//   - no `static` — that would give each TU a PRIVATE copy: the
//     build links, but you've made N copies instead of one shared
//     definition (and module 06 explained why that's a lie here)

#include <cassert>
#include "inline1.h"

int main() {
    const double xs[] = {2.0, 4.0, 6.0, 8.0};

    assert(mean(xs, 4) == 5.0);       // uses THIS TU's view of mean()
    assert(spread(xs, 4) == 6.0);     // stats TU used its own view too

    assert(mean(nullptr, 0) == 0.0);
    return 0;
}

// I AM NOT DONE
