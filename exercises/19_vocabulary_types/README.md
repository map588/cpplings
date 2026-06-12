# 19_vocabulary_types — Saying "maybe", "one of", and "several" in the type system

C++17 shipped a vocabulary for shapes of data every program has but C++
could only fake: a value that might be absent (`optional`), a value
that's one of a closed set of types (`variant`), several values at once
(`tuple` + structured bindings), and a value of *any* type (`any`).

## What this module covers

- **`std::optional<T>`** *(C++17)* — presence in the type, replacing
  sentinel values (`-1`, `""`, `nullptr`) that callers forget to check.
  Access: `has_value()`/`operator bool`, `*opt` (UB if empty!),
  `.value()` (throws `bad_optional_access`), `.value_or(fallback)`.
- **`std::variant<A, B, C>`** *(C++17)* — a type-safe union: exactly one
  alternative alive at a time, and it *knows which*. Query with
  `holds_alternative<T>` / `get_if<T>`; `get<T>` on the wrong
  alternative throws. The closed-set alternative to inheritance
  (module 11): no heap, no vptr, exhaustiveness checked at compile time.
- **`std::visit`** + the *overloaded idiom* — dispatch on the active
  alternative; forget a case and it **doesn't compile** (compare:
  forgetting a virtual override compiles fine, module 11).
- **`std::tuple`** *(C++11)* & **structured bindings** *(C++17)* —
  multiple return values without out-parameters:
  `auto [lo, hi] = minmax(v);` Bindings also unpack pairs (map
  iteration!), arrays, and plain structs.
- **`std::any`** *(C++17)* — a type-erased box for literally anything;
  `any_cast` checks the type at runtime. Rare in APIs — reach for
  `variant` when the set is known.
- **`std::expected<T, E>`** *(C++23 — reading note)* — "value or error",
  the missing piece: like `optional` but the empty state carries *why*.
  Until it's everywhere: `optional` + out-channel, or variants.

## Version notes

| Feature | Standard |
|---|---|
| `std::tuple`, `std::tie` | C++11 |
| `optional`, `variant`, `any`, structured bindings | C++17 |
| `std::expected` | C++23 |
| `std::optional` monadic ops (`and_then`, `transform`) | C++23 |
