# 08_classes2 — The six special members, copying, and the rules of 0/3/5

Every C++ class has up to **six special member functions** the compiler
can write for you:

1. default constructor
2. destructor
3. copy constructor
4. copy assignment
5. move constructor *(C++11)*
6. move assignment *(C++11)*

The compiler-generated versions do the member-wise thing — which is
exactly right until a class *manually manages a resource*, and exactly
wrong after.

## The rules

- **Rule of Zero** — the goal. Hold resources through types that already
  manage themselves (`std::string`, `std::vector`, `unique_ptr`), declare
  none of the six, and the generated ones are all correct.
- **Rule of Three** (pre-C++11) — if you need a destructor, a copy
  constructor, or copy assignment, you almost certainly need *all three*:
  needing one means member-wise semantics are wrong for some resource.
- **Rule of Five** (C++11) — same idea plus the two move operations. Not
  writing them isn't an error; it's a silent *pessimization* (you copy
  where you could have moved).
- **Suppression rules** (the part nobody remembers): declaring a
  *destructor* or any *copy* operation prevents the compiler from
  generating *moves* — `std::move` then silently copies. Declaring a
  *move* operation **deletes** the copies. An innocent `~X() {}` can slow
  a class down; exercise `special1` makes this visible.
- *"Rule of seven"* isn't a thing — you may have heard "six special
  members" or seen `operator<=>` lumped in informally, but the canonical
  guidance is Zero / Three / Five.

## Also in this module

- **Operator overloading** — free-function operators for symmetry, what
  `==` should mean, and **`operator<=>`** *(C++20)*: one defaulted
  spaceship gives you all six comparisons (defaulting `<=>` also implies
  a defaulted `==`), comparing members in declaration order.
- **`friend`** — granting one function access to privates, and the
  *hidden friend* idiom: define the friend inside the class and it's only
  findable via ADL (module 05), keeping the global namespace clean.
- **`static` members** — one per class, not per object; why `static int
  x;` in the class is only a *declaration*, and how `inline static`
  *(C++17)* fixes the link error (module 06's inline-variable rule, again).

## Version notes

| Feature | Standard |
|---|---|
| Copy ctor/assign, `friend`, `static` members, operator overloading | C++98 |
| Move ctor/assign — the six-member roster completed | C++11 |
| `inline static` data members | C++17 |
| `operator<=>`, defaulted comparisons | C++20 |
