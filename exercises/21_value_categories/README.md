# 21_value_categories — lvalue, prvalue, xvalue: the grammar under move semantics

Every C++ expression has a *value category*, and half the language's
behavior keys off it: which overload wins (module 04), what binds to
what (module 03), what can be moved from (module 09), when elision
applies. This module makes the taxonomy precise.

## The taxonomy

```
            expression
            /        \
       glvalue      rvalue
       /     \      /    \
   lvalue    xvalue     prvalue
```

- **lvalue** — has identity, can't be moved from implicitly. Named
  variables, `*p`, `s.member` (of an lvalue), `++x`, string literals,
  and *any function call returning `T&`*.
- **prvalue** — pure value, no identity: literals (`42`), `x + 1`,
  `x++` (note the asymmetry with `++x`!), and calls returning `T` by
  value. Since C++17 a prvalue isn't an object at all — it's a recipe
  that *materializes* directly into its destination (that's why elision
  is "guaranteed": there was never anything to copy).
- **xvalue** ("expiring") — has identity AND may be plundered:
  `std::move(x)`, calls returning `T&&`, members of rvalues.

## The probe

`decltype((expr))` — double parens — encodes the category:
`T&` = lvalue, `T&&` = xvalue, `T` = prvalue.

## Also in this module

- **Function return types as category factories**: `T f()` calls are
  prvalues, `T& f()` calls are lvalues (assignable!), `T&& f()` calls
  are xvalues.
- **Lifetime extension, precisely**: binding `const T&`/`T&&` *directly*
  to a prvalue (or its member subobject) extends the temporary; passing
  through a function call or any other indirection does not.
- **Returning references safely** — a `T&`-returning function is a
  promise about *someone else's* lifetime; returning into a by-value
  parameter is a use-after-free factory.
- **Ref-qualified member functions** *(C++11)* — `f() &` vs `f() &&`:
  overloading on whether `*this` is expiring, so temporaries can donate
  their guts instead of copying.

## Version notes

| Feature | Standard |
|---|---|
| lvalue/rvalue split | C (1970s) |
| xvalues, `std::move` | C++11 |
| Ref-qualified member functions | C++11 |
| Prvalue = "recipe", guaranteed elision, temporary materialization | C++17 |
