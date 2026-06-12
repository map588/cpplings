# 05_namespaces_using — Namespaces, the four faces of `using`, and ADL

If you learned C++ when `using` meant one thing (`using namespace std;`),
this module is the catch-up. The keyword now does four unrelated jobs, and
unqualified name lookup has a famous extra rule (ADL) that makes
`std::cout << x` work at all.

## What this module covers

- **Namespaces.** Scoped names, nesting (`namespace a::b::c {}` compact
  syntax since C++17), **namespace aliases** (`namespace fs =
  std::filesystem;`), and **anonymous namespaces** (the modern spelling of
  file-private — covered in depth in module 06).
- **The four faces of `using`:**
  1. *using-directive* — `using namespace std;` dumps a whole namespace
     into the enclosing scope. Fine inside a small function; in a header
     or at file scope in a big TU it invites silent ambiguities.
  2. *using-declaration* — `using std::swap;` imports ONE name. Precise,
     and a key ingredient of the swap idiom.
  3. *alias-declaration* (C++11) — `using u64 = unsigned long long;`
     replaces `typedef`, reads left-to-right, and unlike typedef can be
     templated: `template<class T> using Grid = vector<vector<T>>;`.
  4. *using-enum-declaration* (C++20) — `using enum Color;` brings scoped
     enumerators into scope (great inside a `switch`).
  (A fifth face, `using Base::Base;` inheriting constructors (C++11),
  appears in the classes modules.)
- **ADL (argument-dependent lookup).** An *unqualified* call `f(x)` also
  searches the namespaces of `x`'s type. This is why `std::cout << x`
  finds `std::operator<<`, and why the customization-point idiom is
  `using std::swap; swap(a, b);` — qualified `std::swap(a, b)` would
  *bypass* your type's own swap.
- **`inline namespace`** — library versioning: the inline member's names
  are visible through the parent, so `lib::checksum` means
  `lib::v2::checksum` while old code can still say `lib::v1::checksum`.

## Version notes

| Feature | Standard |
|---|---|
| Namespaces, using-directives/declarations, ADL | C++98 |
| `inline namespace` | C++11 |
| Alias-declarations (`using X = Y;`) & alias templates | C++11 |
| Scoped enums (`enum class`) | C++11 |
| Compact nested namespaces (`namespace a::b {}`) | C++17 |
| `using enum` | C++20 |
| `std::ranges::swap` (does the swap two-step for you) | C++20 |
