# 12_templates1 — Templates: code the compiler writes for you

A template is not code — it's a *recipe* for code. Nothing exists until
you **instantiate** it with concrete types, at which point the compiler
stamps out a fresh function or class per combination. Understanding that
one fact explains template errors, linker quirks, and bloat.

## What this module covers

- **Function templates & deduction.** `template <typename T>` — the
  compiler deduces `T` from the arguments (the same machinery as `auto`,
  module 01). Deduction needs consistency: `largest(3, 2.5)` won't pick
  a "common type"; it's an error you resolve with an explicit
  `largest<double>(...)`.
- **Class templates.** `Stack<int>` and `Stack<std::string>` are two
  unrelated classes generated from one recipe.
- **Dependent names & `typename`.** Inside a template,
  `std::vector<T>::iterator` *might* be a type or a static member — the
  compiler can't know until `T` arrives, and the rule says: assume "not a
  type" unless you say `typename`. (`auto` makes most of these vanish,
  but you must recognize the error.)
- **Specialization.** `template <> struct Formatter<bool>` — a custom
  implementation for one specific type, replacing the generic recipe.
- **Reading template errors.** The error is almost never *in* the
  template — it's in your type, surfacing at the *instantiation point*.
  Read the "required from here" chain from the bottom up: the last
  "required from here" is your call site; the top is what your type
  failed to provide.
- **Non-type template parameters.** Templates take values, not just
  types: `template <typename T, std::size_t N>` — how array sizes ride
  along in the type system (`std::array`, `std::span`'s fixed extents),
  and how a reference-to-array parameter defeats pointer decay.

## Version notes

| Feature | Standard |
|---|---|
| Templates, specialization, `typename`, NTTPs | C++98 |
| `auto` return & deduction everywhere | C++11/14 |
| `std::size`/`std::ssize` for arrays | C++17/20 |
| Class NTTPs (floating point, literal types) | C++20 |
| Concepts — the error-message fix (module 14) | C++20 |
