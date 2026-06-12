# 06_linkage_odr — Translation units, linkage, and the One Definition Rule

The first module where exercises span **multiple `.cpp` files** — because
linkage errors can't exist inside one. The watcher compiles all the files
listed for the exercise together; some failures here come from the
**linker** (`duplicate symbol`, `undefined symbol`), which looks nothing
like a compiler error. Learn to recognize both.

## What this module covers

- **The build model.** Each `.cpp` + everything it `#include`s = one
  *translation unit* (TU), compiled in total isolation. The linker then
  stitches object files, matching every *use* of a symbol to exactly one
  *definition*.
- **Declaration vs definition.** `extern int x;` declares (no storage);
  `int x;` at namespace scope DEFINES (C++ has no C-style tentative
  definitions — two TUs with `int x;` is a duplicate symbol).
- **Linkage.** *External*: one entity shared across TUs. *Internal*
  (`static`, or an anonymous namespace — the modern spelling): each TU
  gets its own private copy, invisible to the linker.
  Gotcha: namespace-scope `const` variables default to **internal**
  linkage in C++ (the opposite of C!).
- **The ODR.** Every program-wide entity must have exactly one definition —
  except types and `inline` things, which may be defined in many TUs **as
  long as the definitions are token-for-token identical**. If they differ,
  no diagnostic is required: the linker keeps one arbitrarily. That's an
  ODR violation — the nastiest silent bug in C++.
- **`inline`.** Real meaning: "multiple identical definitions allowed; the
  linker folds them." It is *permission for header definitions*, not an
  optimization hint. `constexpr` functions are implicitly inline; class
  members defined in-class are too. **`inline` variables (C++17)** finally
  allow header-defined globals with a single shared address.
- **Include guards.** `#pragma once` (universal in practice) or classic
  `#ifndef`/`#define` guards keep a header from defining the same type
  twice in one TU.

## Version notes

| Feature | Standard |
|---|---|
| `extern`, `static`, ODR, include guards | C++98 (and C before it) |
| Anonymous namespaces preferred over file-`static` | C++98 (entities get internal linkage since C++11) |
| `constexpr` implies `inline` for functions | C++11 |
| `inline` variables | C++17 |
| Modules (`import`) — the eventual successor to all of this | C++20 |
