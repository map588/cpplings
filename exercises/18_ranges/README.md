# 18_ranges — Algorithms that take containers, pipelines that compute lazily

C++20 ranges are two upgrades in one:

1. **Range algorithms** — `std::ranges::sort(v)` instead of
   `std::sort(v.begin(), v.end())`, plus **projections**: a third
   argument that says *which part* of each element to look at.
2. **Views** (`std::views::`) — lazy, composable, non-owning pipeline
   stages: `v | views::filter(p) | views::transform(f)`.

## What this module covers

- **Range algorithm overloads.** Whole containers as arguments;
  constrained with concepts (module 14), so error messages name the
  unmet requirement.
- **Projections.** `ranges::sort(people, {}, &Person::age)` — sort
  people *by age* with no comparator lambda. `{}` is the default
  comparator (`std::less`); the projection applies before comparison.
  Most `ranges::` algorithms take one.
- **Views & laziness.** A view pipeline does *no work when built* —
  computation happens element-by-element **during iteration**, and only
  for the elements you actually consume. `take(3)` on a filtered,
  transformed range evaluates just enough input to yield 3 outputs.
- **Infinite ranges.** `views::iota(1)` counts forever; laziness makes
  that fine — bound it downstream with `take`.
- **Dangling protection.** Views don't own (string_view rules, module
  15) — but the library *catches* the worst case: a range algorithm
  called on an rvalue container returns `std::ranges::dangling`, a type
  you can't dereference. The bug becomes a compile error.

## Version notes

| Feature | Standard |
|---|---|
| `<ranges>`, `std::ranges::` algorithms, projections, `views::` | C++20 |
| `views::zip`, `views::enumerate`, `ranges::to<vector>()` | C++23 — reading note: `ranges::to` finally makes collecting a pipeline into a container a one-liner; until then, construct from `begin()`/`end()` or loop |
