# 03_references_pointers — Aliases, addresses, and lifetimes

References and pointers both let two names reach one object. The
differences are about *guarantees*: what can be null, what can be reseated,
and what the compiler enforces about lifetime.

## What this module covers

- **References are aliases, not objects.** A reference must be initialized,
  can never be null, and can never be reseated — assigning to it assigns
  *through* it to the referent. There is no "reference arithmetic" and no
  `&` of "the reference itself."
- **Binding rules.** A non-const lvalue reference (`T&`) can only bind to a
  modifiable lvalue. A `const T&` can additionally bind to a *temporary* —
  and **extends that temporary's lifetime** to the reference's own scope.
  This one rule is why `const&` parameters are everywhere.
- **Dangling.** The compiler does not track lifetimes for you (that's the
  feature Rust built a language around). Returning a reference/pointer to a
  local, or keeping one past the owner's death, compiles fine and is
  undefined behavior. Sanitizers (ASan/UBSan) make these visible at runtime.
- **`nullptr` (C++11).** A dedicated null of type `std::nullptr_t`. `NULL`
  is just the integer 0 in disguise, which breaks overload resolution
  (`f(int)` vs `f(int*)` — `f(NULL)` picks the int!). Always `nullptr`.
- **Choosing a parameter type.** Value for cheap things, `const T&` for
  read-only access to expensive things, `T&` for in/out parameters,
  pointer when "absent" is a legal argument.

## Version notes

| Feature | Standard |
|---|---|
| `nullptr`, `std::nullptr_t` | C++11 |
| Rvalue references `T&&` (next module!) | C++11 |
| `std::reference_wrapper` (reseatable, container-storable "reference") | C++11 |
| Lifetime extension via `const T&` / `T&&` binding | C++98 / C++11 |
