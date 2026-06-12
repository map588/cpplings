# 24_compile_time — Programs that run before they exist

Module 02 introduced `constexpr`; this module is what it grew into: type
introspection, compile-time data structures, lookup tables baked into
the binary, and values as template parameters.

## What this module covers

- **Type traits** (`<type_traits>`, C++11; `_v`/`_t` shorthands C++17) —
  the introspection library: `is_trivially_copyable_v`, `is_same_v`,
  `remove_cvref_t`... As `static_assert` *gates*, they turn "this
  function is only safe for certain types" into compile errors with
  human-written messages.
- **Compile-time tables.** A `consteval` function returning `std::array`
  runs entirely at build time — the table ships as initialized data,
  zero runtime cost. (`consteval` — *must* run at compile time — is the
  right tool; `constexpr` would silently fall back to runtime if you
  fumbled a detail.)
- **`constexpr` containers** *(C++20)* — `std::vector` and `std::string`
  work inside constant evaluation (allocate, use, free — all at compile
  time), as long as no allocation *escapes* to runtime. Parse-at-build,
  return counts/arrays.
- **Class-type NTTPs** *(C++20)* — whole structs as template parameters:
  `Buffer<Config{.size = 64}>`. Each distinct value is a distinct type;
  invalid configs can be `static_assert`ed inside the template.
- **`std::is_constant_evaluated()`** *(C++20)* — "am I running at
  compile time?" — with the legendary footgun: in `if constexpr` it is
  ALWAYS true (the condition itself is constant-evaluated!). Use a
  plain `if`; C++23's `if consteval` exists because everyone fell in
  this hole.

## Version notes

| Feature | Standard |
|---|---|
| `<type_traits>` | C++11 (`_v` variables C++17) |
| Relaxed `constexpr` (loops, locals) | C++14 |
| `constexpr` lambdas, `if constexpr` | C++17 |
| `consteval`, `constinit`, constexpr containers, class NTTPs, `is_constant_evaluated` | C++20 |
| `if consteval` | C++23 |
