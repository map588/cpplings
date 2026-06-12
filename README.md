# cpplings 🐉

Learn deep C++ by fixing small, broken programs — in the spirit of
[rustlings](https://github.com/rust-lang/rustlings) and
[ziglings](https://codeberg.org/ziglings/exercises/).

This is not a beginner syntax course. It's aimed at people who *write* C++
already but want to actually know the lay of the land: why a few extra
keywords (`static`, `const&`, `constexpr`, `noexcept`, `[[no_unique_address]]`...)
drastically change what the compiler does, where undefined behavior actually
hides, and what each standard from C++11 to C++23 added or took away.

## Setup

You need:

- A C++20 compiler — recent `clang++` (incl. Apple clang) or `g++`
- Python **3.11+** (the runner is one stdlib-only script)

```sh
python3 cpplings.py        # start the watcher and begin
```

The watcher finds your first unfinished exercise, compiles and runs it on
every save, and shows the result. Type `h` + Enter for a progressive hint,
`q` + Enter to quit. When an exercise passes, delete its `// I AM NOT DONE`
line to advance.

Other commands:

```sh
python3 cpplings.py list             # progress overview
python3 cpplings.py run types3       # build & run one exercise
python3 cpplings.py hint types3      # all hints for one exercise
python3 cpplings.py verify           # re-check everything in order
python3 cpplings.py check-solutions  # maintainers/CI: solutions pass, exercises fail
```

Environment knobs: `CPPLINGS_CXX=g++-14` to pick a compiler,
`CPPLINGS_NO_SANITIZERS=1` if ASan/UBSan misbehave on your platform
(they're on by default — several exercises *rely* on them to make invisible
bugs visible).

## How exercises verify themselves

Every exercise is a single `.cpp` that is broken on purpose. It passes when
it compiles **with `-Wall -Wextra`** and exits 0 — meaning its `assert`s and
`static_assert`s hold and the sanitizers found nothing. Solutions live in
`solutions/`, mirroring the layout; hints first, solutions second.

## Version annotations

Since the language grew in big steps (C++11 → 14 → 17 → 20 → 23), every
feature is tagged with the standard that introduced it the first time it
appears, like `string_view (C++17)` — and removals/deprecations are called
out too (`auto_ptr` removed in C++17). Everything *compiles* as C++20; the
tags are there so you can read any codebase's vintage. C++23 features appear
as short reading notes until compiler support is everywhere.

## Curriculum

Six parts, 27 modules, **148 exercises** — from initialization rules to
move semantics, templates & concepts, value categories, UB & sanitizers,
memory layout, and atomics. See [CURRICULUM.md](CURRICULUM.md) for the
full map.

| Part | Theme |
|---|---|
| I | Foundations: types, const/constexpr, references, linkage |
| II | Classes & ownership: RAII, move semantics, polymorphism |
| III | Generic programming: templates, lambdas, concepts |
| IV | The standard library: containers, algorithms, ranges, vocabulary types |
| V | Correctness: exceptions, value categories, undefined behavior |
| VI | Under the hood: memory layout, compile-time programming, concurrency |
