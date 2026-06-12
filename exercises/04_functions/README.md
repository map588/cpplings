# 04_functions — Overloads, defaults, and `static` locals

Functions look like the simple part of C++. Then overload resolution picks
something you didn't expect, a default argument gets evaluated more often
than you thought, and a local variable outlives the call.

## What this module covers

- **Overload resolution.** When several `f`s could match, the compiler ranks
  the conversions each call would need:
  1. **Exact match** (incl. trivial things like `T` → `const T&`)
  2. **Promotion** — `char`/`short`/`bool` → `int`, `float` → `double`
  3. **Conversion** — everything else: `int` → `double`, `long` → `int`,
     `int` → `unsigned`, pointer → `bool`...
  A promotion always beats a conversion. Two equally-ranked candidates =
  **ambiguous**, a compile error — the compiler never flips a coin.
- **Reference overloads.** `f(int&)` vs `f(const int&)` vs `f(int&&)`:
  lvalues pick `int&`, const lvalues pick `const int&`, rvalues prefer
  `int&&` (falling back to `const int&` if absent). This ranking is the
  mechanism move semantics rides on (module 09).
- **Default arguments.** Must be trailing; they're substituted at the *call
  site* and **re-evaluated on every call** — a default of `next_id()` calls
  it each time.
- **`static` locals.** Initialized once, on first execution of the line
  (lazily!), alive until program exit. Since C++11 that initialization is
  thread-safe ("magic statics").

## Version notes

| Feature | Standard |
|---|---|
| Overload resolution & default arguments | C++98 |
| Rvalue-reference overloads (`T&&`) | C++11 |
| Thread-safe (magic) static-local initialization | C++11 |
| `std::cmp_*` to sidestep mixed-sign ambiguity traps | C++20 |
