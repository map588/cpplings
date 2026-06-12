# 09_move_semantics — Theft, casts that don't move, and moves that don't happen

The C++11 feature everyone names first, and the most misunderstood. The
mental model that survives contact with the standard:

> A move is a **copy that's allowed to vandalize the source**. `std::move`
> performs no move — it's a *cast* that marks an object as vandalizable.
> And the fastest move is the one the compiler *elides* entirely.

## What this module covers

- **`std::move` is `static_cast<T&&>`.** It changes which overload wins
  (module 04's `&`/`const&`/`&&` ranking) — nothing else. Moving a
  `std::string` steals its heap buffer; the *source keeps its allocation
  identity* (provable by watching `.data()` pointers).
- **Moved-from state**: *valid but unspecified*. You may destroy it,
  assign to it, call anything with no preconditions. You may not assume
  contents.
- **Writing move operations** (rule of five): steal the guts, null the
  source, and mark them **`noexcept`** —
- **The `noexcept` payoff**: `std::vector` reallocation uses
  `move_if_noexcept`. A throwing-move type gets *copied* on every growth
  (the strong exception guarantee demands it). One missing keyword,
  quadratic regret.
- **Copy elision.** Since C++17, returning a prvalue (`return T{...};`)
  is **guaranteed** to construct directly in the caller — zero copies,
  zero moves, by language rule. Named locals (NRVO) are elided in
  practice but not guaranteed. Corollary: `return std::move(local);` is a
  *pessimization* — it disables elision and forces a move
  (`-Wpessimizing-move` warns).
- **Sink parameters.** `void set(T value)` + `member_ = std::move(value)`
  handles lvalues (one copy + one move) and rvalues (zero copies) with
  one overload — usually beating `const T&` for "store this" APIs.
- **Perfect forwarding.** In `template <class T> void f(T&& x)`, `T&&` is
  a *forwarding reference* (deduced!): lvalues come in as `T&`, rvalues
  as `T&&` (reference collapsing). Pass it on with `std::forward<T>(x)` —
  `std::move` there would plunder your caller's lvalues.
- **The const trap:** `std::move(const_thing)` yields `const T&&`, which
  no move constructor takes — overload resolution falls back to the copy
  constructor. Compiles, runs, silently copies.

## Version notes

| Feature | Standard |
|---|---|
| Rvalue references, `std::move`, `std::forward`, move members | C++11 |
| `move_if_noexcept` vector behavior | C++11 |
| **Guaranteed** copy elision for prvalues | C++17 |
| `-Wpessimizing-move`/`-Wredundant-move` diagnostics | compilers, not the standard |
