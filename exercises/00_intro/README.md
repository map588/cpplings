# 00_intro — How this all works

Welcome to **cpplings**. Each exercise is a single `.cpp` file that is broken
on purpose — it fails to compile, or compiles but fails its own `assert`s.
Your job is to make it pass, then delete the `// I AM NOT DONE` line to move on.

Run the watcher from the repo root and it will walk you through in order:

```sh
python3 cpplings.py
```

Type `h` + Enter for a hint, `q` + Enter to quit. Solutions live in
`solutions/` if you're truly stuck — but try the hints first.

## What this module covers

- **The compilation model.** A C++ "build" is four phases: *preprocessing*
  (textual `#include`/macro expansion), *compilation* (one translation unit →
  assembly), *assembly* (→ object file), and *linking* (object files →
  executable). Errors read very differently depending on which phase produced
  them, and learning to tell them apart is half of debugging C++.
- **Reading compiler errors.** Always read the *first* error; later ones are
  often cascade noise.
- **The feedback loop.** Exercises verify themselves with `assert` (runtime,
  from `<cassert>`) and `static_assert` (compile time — C++11, message
  optional since C++17).

## Version notes

| Feature | Standard |
|---|---|
| `static_assert(cond, "msg")` | C++11 |
| `static_assert(cond)` — no message | C++17 |
| `constexpr` functions | C++11 (heavily relaxed in C++14) |
