# 14_templates2 — Concepts, packs, and compile-time `if`

Module 12 showed the old world: templates accept anything, and unmet
expectations explode *inside* the template. This module is the modern
toolkit: constraints that turn those explosions into one-line errors,
parameter packs that replace overload ladders, and `if constexpr`
branching at compile time.

## What this module covers

- **Concepts** *(C++20)* — named, checkable requirements on template
  parameters:
  ```cpp
  template <typename T> concept Numeric =
      std::integral<T> || std::floating_point<T>;

  template <Numeric T> T average(const std::vector<T>&);
  ```
  An unsatisfied concept fails at the *call site* with "constraint not
  satisfied", not forty lines into someone else's header.
- **`requires` expressions** — describe an interface structurally:
  ```cpp
  template <typename T> concept Drawable = requires(T t) {
      { t.draw() } -> std::convertible_to<std::string>;
  };
  ```
- **Constrained overloading.** Several templates, different concepts —
  the *most constrained* match wins (subsumption). This retires C++17-era
  SFINAE/`enable_if` and tag-dispatch.
- **Variadic templates** *(C++11)* — `typename... Ts` parameter packs;
  `sizeof...(args)`; expansion patterns. **Fold expressions** *(C++17)*:
  `(args + ...)`, `(args && ...)`, and the comma-fold
  `(f(args), ...)` that applies a function to every pack element.
- **CTAD** *(C++17)* — class template argument deduction: `std::pair
  p{1, 2.5}`, `std::vector v{1, 2, 3}`. **Deduction guides** steer it
  when the constructor argument shouldn't be taken literally
  (`Box("hi")` → `Box<std::string>`, not `Box<const char*>`).
- **`if constexpr`** *(C++17)* — a compile-time branch whose untaken arm
  is *not instantiated* for the current `T`. One function body, multiple
  type-specific behaviors, no specialization ladder.

## Version notes

| Feature | Standard |
|---|---|
| Variadic templates, `sizeof...` | C++11 |
| Fold expressions, CTAD, `if constexpr` | C++17 |
| Concepts, `requires`, abbreviated templates (`auto` params) | C++20 |
| Deduction guides for aggregates (no guide needed) | C++20 |
