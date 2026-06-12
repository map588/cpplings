# 13_lambdas — Anonymous functions that are secretly classes

If you've never used lambdas: there is no magic. A lambda expression is
compiler shorthand for a **class with an `operator()`** (a *closure
type*), instantiated on the spot:

```cpp
auto add5 = [n](int x) { return x + n; };
// ≈ struct __anon { int n;  int operator()(int x) const { return x + n; } };
//   __anon add5{n};
```

Every concept in this module falls out of that desugaring: captures are
*members*, `mutable` removes the `const` from `operator()`, each lambda
is its own unique type, and storage/cost questions are just questions
about a small struct.

## Anatomy

```cpp
[capture_list](parameters) -> return_type { body }
//  ^ what outer state the closure carries (members)
//                          ^ usually deduced; rarely written
```

## What this module covers

- **Captures.** `[x]` copies x *at the point of creation* (a member,
  initialized then); `[&x]` stores a reference. Defaults `[=]`/`[&]`
  exist; prefer naming what you take. **Init-captures** *(C++14)*:
  `[n = 0]`, `[s = std::move(big)]` — members with initializers, the way
  to *move* something into a closure.
- **`mutable`.** `operator()` is `const` by default — value-captured
  state is read-only until you say `mutable`. The state lives in the
  closure object: copy the lambda, copy the state.
- **Dangling captures.** `[&]` stores references that do not keep
  anything alive — returning such a lambda, or holding one across a
  vector reallocation, is module-03 dangling all over again.
- **Lambdas + algorithms** — the reason lambdas exist. `std::sort` with
  a comparator, `std::count_if` with a predicate; **generic lambdas**
  *(C++14)*: `[](const auto& x)` — an `operator()` *template*.
- **Storage & cost.** A captureless lambda converts to a plain function
  pointer and its closure is empty (`sizeof == 1`); captures make it
  exactly as big as its members. `auto` holds the exact closure type
  (zero overhead, inlinable); `std::function` *(C++11)* type-erases —
  one size fits all callables, paid for with indirection and possible
  allocation. Use `std::function` for *storing* heterogeneous callbacks,
  `auto`/templates for everything else.

## Version notes

| Feature | Standard |
|---|---|
| Lambdas, `std::function` | C++11 |
| Init-captures, generic lambdas (`auto` params), `auto` return | C++14 |
| `constexpr` lambdas, `*this` capture | C++17 |
| Template-parameter lambdas `[]<typename T>(T x)`, captureless default-constructible | C++20 |
