# 02_const_constexpr — Promises to the compiler

`const` and `constexpr` look like decoration, but they change what the
compiler is allowed to assume, what your API promises, and what runs at
compile time instead of runtime.

## What this module covers

- **`const` correctness.** `const` on a variable means "no one mutates this
  through this name." `const` on a member function means "calling this
  doesn't mutate the object" — and is the *only* thing callable through a
  `const` reference. `const` propagates through an API: one missing `const`
  deep in a call chain forces non-const all the way up.
- **`const` and pointers.** Read declarations right-to-left:
  `const int* p` — pointer to const int (the int is protected);
  `int* const p` — const pointer to int (the pointer is frozen);
  `const int* const p` — both.
- **`constexpr` (C++11).** On a *variable*: must be initialized by a constant
  expression — a compile-time value, usable in array bounds, template
  arguments, `static_assert`. On a *function*: it is *allowed* to run at
  compile time when given constant arguments — and still works at runtime.
  `const` alone does NOT mean compile-time: `const int x = rand();` is fine.
- **`consteval` (C++20).** Like `constexpr` but *must* run at compile time —
  calling it with a runtime value is a compile error.
- **`constinit` (C++20).** Forces compile-time *initialization* of a
  static/global without making the variable itself const. Kills the "static
  initialization order fiasco" for that variable.
- **`volatile` (inherited from C).** The *other* qualifier: every read/write
  in the source is a real access, emitted in order — for memory-mapped I/O
  and signal handlers. It is **not** a thread-synchronization tool (that's
  `std::atomic`, C++11). Like const, dropping it silently is forbidden, and
  it propagates through APIs the same way.

## Version notes

| Feature | Standard |
|---|---|
| `constexpr` (variables & single-return functions) | C++11 |
| `constexpr` functions with loops, branches, locals | C++14 |
| `constexpr` lambdas, `if constexpr` | C++17 |
| `consteval`, `constinit`, `constexpr` virtual / try / `std::vector` in constexpr | C++20 |
| `mutable` members (escape hatch from const) | C++98 |
| Compound assignment on `volatile` lvalues deprecated | C++20 |
