# 26_modern_extras — chrono, filesystem, format, coroutines — and graduation

The last module: the modern utility layer every C++ program touches,
plus a first contact with coroutines, closing notes on what C++23
brings, and a graduation quiz.

## What this module covers

- **`<chrono>`** *(C++11)* — durations with the unit in the **type**:
  `std::chrono::seconds`, `milliseconds`, literals (`90min`, `250ms` —
  C++14). Conversions that lose precision require explicit
  `duration_cast`; lossless ones are implicit. The cure for every
  "timeout: int — er, seconds? millis?" API bug.
- **`<filesystem>`** *(C++17)* — `fs::path` with structured
  decomposition (`stem()`, `extension()`, `parent_path()`), `/` as the
  join operator, and the whole directory/status toolkit.
- **`std::format`** *(C++20)* — type-safe, locale-independent
  formatting: `std::format("{}: {:.1f}%", name, pct)`. Replaces
  iostream manipulator soup and printf's type danger. **`std::print`**
  *(C++23 — reading note)* puts it straight on stdout.
- **Coroutines** *(C++20)* — functions that suspend (`co_yield`,
  `co_await`, `co_return`) and resume. The language ships the
  *mechanism* (promise types, handles); the *library* types arrive in
  C++23 (`std::generator`). You'll write a generator-backed coroutine
  against provided machinery — enough to read coroutine code in the
  wild.
- **Modules** *(C++20 — reading note)* — `import std;` instead of
  textual `#include`; kills the ODR/include-guard problem class
  (module 06) at the root. Tooling (CMake + recent compilers) became
  practical ~2024; single-file exercise runners (hello) can't
  demonstrate them. Know they exist, adopt when your build system says
  it's ready.

## Version notes

| Feature | Standard |
|---|---|
| `<chrono>` | C++11 (literals C++14, calendar/time-zones C++20) |
| `<filesystem>` | C++17 |
| `std::format`, coroutines, modules | C++20 |
| `std::print`, `std::generator`, `import std;` | C++23 |
