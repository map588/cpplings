# 20_errors — Exceptions, unwinding, and the guarantees you owe your callers

C++ error handling is two intertwined mechanisms: exceptions (the
language feature) and RAII (the idiom that makes them survivable). This
module covers both, plus the vocabulary for promising how much state
survives a failure.

## What this module covers

- **Throw by value, catch by `const&`.** Catching by value *slices*
  (module 11!) — a derived exception caught as `std::exception` loses
  its message and identity. `catch (const std::exception& e)`, always.
- **Stack unwinding & RAII.** A flying exception runs destructors of
  every local between `throw` and `catch` — RAII resources release
  themselves; naked `new`/manual cleanup leaks. Exception safety isn't
  try/finally discipline (C++ has no finally — on purpose); it's
  ownership discipline.
- **`noexcept`** *(C++11)*. A *promise*, checked at runtime: throwing
  out of a `noexcept` function calls `std::terminate` — no unwinding to
  the caller, no recovery. Mark moves/swaps (module 09's vector story)
  and genuinely infallible code; never mark "I hope it doesn't throw".
  Destructors are implicitly `noexcept`. The `noexcept(expr)` operator
  queries the promise at compile time.
- **Exception-safety guarantees** — the contract vocabulary:
  - *nothrow*: cannot fail
  - *strong*: fails → state unchanged (commit-or-rollback)
  - *basic*: fails → state valid but unspecified, nothing leaked
  - *none*: fails → broken invariants (i.e., a bug)
  The strong guarantee's recipe: **do all throwing work on the side,
  then commit with nothrow operations** (swap/move).
- **Choosing a mechanism.** Exceptions for *exceptional* failures that
  skip many layers; `optional`/`expected` for *routine* absence
  (module 19); error codes at C boundaries. Using exceptions as
  control flow for expected cases is both slow (throwing is expensive;
  the happy path is free) and illegible.
- **`errno` and `<system_error>`** *(C++11)*. Syscalls report failure
  by return value plus thread-local `errno` — meaningful only
  *immediately* after the failed call; the next libc call may overwrite
  it. `std::error_code{errno, std::generic_category()}` gives the raw
  int a type, a `message()`, and portable comparison against the
  `std::errc` enumerators — the vocabulary of `filesystem`'s
  non-throwing overloads and of exception-free codebases.

## Version notes

| Feature | Standard |
|---|---|
| Exceptions, unwinding, `std::exception` hierarchy | C++98 |
| `noexcept` (specifier & operator) | C++11 (`throw()` removed C++20) |
| `<system_error>`: `error_code`, `errc`, categories | C++11 |
| `[[nodiscard]]` — make callers look at error returns | C++17 |
| `std::expected` | C++23 |
