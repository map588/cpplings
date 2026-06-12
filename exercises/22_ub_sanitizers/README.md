# 22_ub_sanitizers — Undefined behavior is a contract, not a crash

UB is the deal underneath C++'s performance: *you promise* never to
overflow a signed int, read out of bounds, or touch dead objects — and
the optimizer *builds on that promise*. Code with UB isn't "code that
crashes"; it's code the compiler was allowed to assume impossible, so
anything the optimizer derived from that assumption is now fiction.
That's why UB bugs appear only at `-O2`, only on Tuesdays: the behavior
was never defined to begin with.

This module makes seven major UB classes *visible* using the sanitizers
this repo always compiles with:

- **UBSan** (`-fsanitize=undefined`) — instruments arithmetic, shifts,
  null derefs, misaligned access; pinpoints the exact line.
- **ASan** (`-fsanitize=address`) — shadow-memory bookkeeping for heap/
  stack: out-of-bounds, use-after-free, use-after-scope. ~2x slowdown,
  worth every cycle in tests/CI.
- (Cousins: **TSan** for data races — module 25; **MSan** for
  uninitialized reads, Linux-only.)

## The UB classes covered

| Exercise | UB | The promise you broke |
|---|---|---|
| ub1 | signed integer overflow | `int` arithmetic never exceeds INT_MAX (unsigned wraps; signed does NOT — module 01) |
| ub2 | out-of-bounds access | every index is within `[0, size)` — `[]` does not check |
| ub3 | use-after-scope | objects die at `}`, even though the stack bytes linger |
| ub4 | misaligned / type-punned access | a `T*` points at a real, aligned `T` (strict aliasing) — `memcpy`/`bit_cast` *(C++20)* are the legal transmuters |
| ub5 | integer division by zero | divisors are nonzero (float division gives ±inf instead — different rules!) |
| ub6 | invalid shifts | shift amounts are in `[0, width)` (and watch promotion: `1 << 31` is already trouble) |

## Version notes

| Feature | Standard / tool |
|---|---|
| Sanitizers | compiler tech (clang/gcc), not the standard |
| `std::bit_cast` — legal type punning | C++20 |
| Signed overflow still UB (two's complement guaranteed since C++20 — representation, not arithmetic!) | C++20 |
| `std::ssize`, checked `.at()` | C++20 / C++98 |
