# 28_performance — compiler options, hints, SIMD, inline asm

What the optimizer does with your code, and the knobs that steer it.
These exercises build with `-O2` (optimization is the subject), and
they verify with asserts, never with a stopwatch — the lesson is the
*mechanism*, not a flaky timing number. The golden rule throughout:
**measure before you reach for any of this.**

## What this module covers

- **Optimization levels** — `-O0`/`-Og`/`-O1`/`-O2`/`-O3`/`-Os` and
  the sharp extras: `-march=native` (SIGILL on older CPUs),
  `-flto`, `-ffast-math` (breaks IEEE — `x != x` stops detecting
  NaN). Probes you can run: `__OPTIMIZE__` exists, `-O2` does *not*
  define `NDEBUG`.
- **Branch hints** — `[[likely]]`/`[[unlikely]]` *(C++20)*,
  `__builtin_expect`; hints never change semantics. And the hint
  with teeth: `__builtin_unreachable()` *(std::unreachable C++23)*
  is a promise, not a check — a lie is UB, and UBSan catches it.
- **`inline`** — an ODR permission slip (module 06), not a speed
  knob; inlining is the optimizer's decision. `[[gnu::always_inline]]`
  / `[[gnu::noinline]]` when you must overrule it. Live duplicate-
  symbol link error to fix.
- **Aliasing & `__restrict__`** — why possible pointer overlap kills
  vectorization; the restrict promise; memcpy (restrict params, UB
  on overlap — ASan's memcpy-param-overlap) vs memmove.
- **SIMD, three roads** — auto-vectorization; portable clang/gcc
  vector extensions (`vector_size` types: lane-wise algebra,
  `__builtin_shufflevector`, log2(n) reductions); real intrinsics
  with a NEON path and an SSE path (and the classic tail-loop bug).
- **Inline asm** — extended-asm constraints (`"r"`, `"=r"`, early-
  clobber `"=&r"`, `"memory"`), dual-architecture; and the empty-asm
  idiom every benchmark harness is built on: `DoNotOptimize` /
  `ClobberMemory`.

## Version notes

| Feature | Standard |
|---|---|
| `[[likely]]` / `[[unlikely]]` | C++20 |
| `std::unreachable` | C++23 (here: `__builtin_unreachable`) |
| `__restrict__`, vector extensions, extended asm | compiler extensions (clang/gcc), not ISO C++ |
| NEON / SSE intrinsics | per-architecture headers |
