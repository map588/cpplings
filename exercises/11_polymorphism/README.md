# 11_polymorphism — Virtual dispatch, what it costs, and how it silently fails

`virtual` buys you runtime dispatch: call through a base reference, run
the derived implementation. This module covers the machinery (vtables and
their costs), the three classic silent failures (hiding instead of
overriding, non-virtual destructors, slicing) and the keywords that turn
them into compile errors.

## What this module covers

- **Dispatch mechanics.** A class with any virtual function carries a
  hidden *vptr* (one pointer per **object**) pointing at the class's
  *vtable* (one table per **class**). A virtual call = load vptr, index
  table, call through pointer — cheap, but it can't inline and only
  happens through a *reference or pointer*. Call on a value and the
  compiler dispatches statically.
- **`override`** *(C++11)*. A derived function with a slightly different
  signature (missed `const`, `int` vs `long`...) doesn't override — it
  *hides*, and the base version keeps running. `override` makes the
  compiler check your claim. Write it on every overriding function,
  without exception. **`final`** *(C++11)* forbids further overriding (and
  enables devirtualization).
- **Virtual destructors.** `delete` through a base pointer with a
  non-virtual destructor is *undefined behavior* — in practice the
  derived destructor never runs. Rule: a base class with virtual
  functions gets `virtual ~Base() = default;`. (Conversely: a class not
  meant to be a polymorphic base shouldn't pay for it.)
- **Pure virtual & abstract classes.** `= 0` means "no default; deriving
  classes MUST implement" — and turns *forgot to implement* from a
  runtime surprise into a compile error. A class with any pure virtual
  cannot be instantiated.
- **Object slicing.** Copying a `Derived` into a `Base`-typed slot
  (by-value parameter, `vector<Base>`...) copies only the Base part —
  the derived data and behavior are sheared off. Polymorphism requires
  indirection: `Base&`, `Base*`, or `vector<unique_ptr<Base>>` (module 10).
- **When *not* to inherit.** Inheritance is for *interface substitution*.
  For code reuse, prefer composition; for "one of N shapes", consider
  `std::variant` (module 19) — no heap, no vptr.

## Version notes

| Feature | Standard |
|---|---|
| `virtual`, pure virtuals, abstract classes | C++98 |
| `override`, `final` | C++11 |
| `dynamic_cast` & RTTI | C++98 (costs covered later) |
| `std::variant` as a closed-set alternative | C++17 |
