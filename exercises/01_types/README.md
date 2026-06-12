# 01_types — Initialization, deduction, and the integer rules nobody told you

C++ inherited C's arithmetic rules and then layered four decades of
initialization syntax on top. This module covers the traps that result.

## What this module covers

- **Initialization forms.** `int x = 5;` (copy-init), `int x(5);`
  (direct-init), and `int x{5};` (*list-initialization*, C++11 — often called
  uniform or brace init). The crucial difference: **brace init rejects
  narrowing conversions** at compile time. `int x = 3.9;` silently truncates;
  `int x{3.9};` does not compile. This is why modern style favors braces.
- **`auto` deduction (C++11).** `auto` uses template-argument-deduction rules:
  it drops top-level `const` and references, and decays arrays/functions to
  pointers. `auto&`, `const auto&`, and `auto&&` each deduce differently.
- **Integral promotion.** Anything smaller than `int` (`char`, `short`,
  `uint8_t`...) is promoted to `int` before arithmetic. `uint8_t + uint8_t`
  is an `int` — no wraparound happens unless *you* truncate the result.
- **Signed/unsigned conversions.** Comparing or mixing signed and unsigned
  converts the signed operand to unsigned (`-1 > 1u` is *true*). Unsigned
  arithmetic wraps (well-defined); **signed overflow is undefined behavior**.
- **Fixed-width types** (`<cstdint>`, C++11). Plain `int` only guarantees
  ≥16 bits; when width matters, say so.

## Version notes

| Feature | Standard |
|---|---|
| List/brace initialization, narrowing rejection | C++11 |
| `auto` type deduction | C++11 |
| `<cstdint>` fixed-width types | C++11 |
| Integer division/modulo defined to truncate toward zero | C++11 (was impl-defined) |
| `std::ssize` (signed size) | C++20 |
| `std::cmp_less` & friends — safe mixed-sign comparison | C++20 |
| Signed integers guaranteed two's complement | C++20 |
