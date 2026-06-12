# 07_classes1 — Construction, destruction, and the conversions you didn't ask for

How objects come to life: initializer lists vs assignment, defaults,
delegation, the `explicit` keyword that prevents silent conversions, and
the precisely-defined order in which everything dies.

## What this module covers

- **Member init lists.** `Ctor() : member_(x)` *initializes*; assignment in
  the body default-constructs first, then assigns — wasteful for big types
  and **impossible** for `const` and reference members. Init order follows
  *declaration* order, not list order (`-Wreorder` warns when they differ).
- **Default member initializers** (NSDMI, C++11) — `int n_ = 0;` right at
  the declaration; used by every constructor that doesn't override it.
- **Delegating constructors** (C++11) — `Timer(int s) : Timer(s, false) {}`
  — one constructor does the work, the rest forward.
- **`explicit`.** A one-argument constructor is an *implicit conversion*
  from that argument type unless marked `explicit`. Implicit conversions
  through constructors are how `f(5)` silently becomes
  `f(Minutes{5})` — or the wrong unit entirely.
- **Destructors & order.** Locals destroy in reverse construction order;
  members destroy in reverse declaration order; bases after members. This
  ordering is what makes RAII composable (module 10).
- **`= default` / `= delete`** (C++11). Writing *any* constructor
  suppresses the implicit default constructor — `= default` asks for it
  back. `= delete` removes a function from overload resolution entirely —
  including, usefully, an overload that would otherwise be reached by an
  unwanted implicit conversion.
- **Aggregates & designated initializers.** A struct with no user
  constructors is an *aggregate*: `{}`-initializable member-wise, and
  since C++20 with names: `Config{.port = 9000}` (declaration order
  required!).

## Version notes

| Feature | Standard |
|---|---|
| Member init lists, `explicit`, aggregates | C++98 |
| NSDMI, delegating ctors, `=default`/`=delete` | C++11 |
| `explicit(bool)` conditional explicit | C++20 |
| Designated initializers (`.x = 1`) | C++20 (C had them since C99!) |
| Parenthesized aggregate init `Config(1, 2)` | C++20 |
