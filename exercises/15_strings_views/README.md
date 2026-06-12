# 15_strings_views — std::string's secrets and the view that doesn't own

## What this module covers

- **SSO — the small string optimization.** Short strings live *inside*
  the `std::string` object itself (22 chars on libc++, 15 on libstdc++);
  only longer ones allocate. Consequences: short strings are cheap to
  copy, *moving* them copies anyway (nothing to steal — module 09), and
  `.data()` can point into the object on the stack.
- **`std::string_view`** *(C++17)* — a `{pointer, length}` pair viewing
  characters it does **not own**. `substr`, `remove_prefix`, comparisons:
  all O(1) or allocation-free. The default parameter type for read-only
  string arguments — it binds to `std::string`, literals, and char
  arrays alike without copying.
- **The lifetime price.** A view into a destroyed string is a dangling
  pointer with a length attached. Temporaries are the classic source:
  `std::string_view sv = s + "!";` dangles at the semicolon. clang's
  `-Wdangling-gsl` catches some patterns; ASan catches the rest at use.
  Rule: a `string_view` *parameter* is almost always fine (the argument
  outlives the call); a `string_view` *member or return value* needs a
  documented owner.
- **Number conversions.** `std::to_string`, `std::stoi` (throws,
  locale-aware) and the modern `std::from_chars` *(C++17)* —
  locale-free, non-throwing, non-allocating, fastest available.

## Version notes

| Feature | Standard |
|---|---|
| `std::string_view` | C++17 |
| `std::from_chars` / `std::to_chars` | C++17 |
| `starts_with` / `ends_with` | C++20 |
| `contains` | C++23 |
