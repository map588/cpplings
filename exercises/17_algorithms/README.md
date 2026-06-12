# 17_algorithms — Stop writing raw loops

`<algorithm>` and `<numeric>` are ~100 pre-written, pre-debugged,
named loops. "No raw loops" is half style advice and half bug repellent:
an algorithm's name states intent, its implementation handles the edge
cases you'd have flubbed.

## What this module covers

- **The iterator-pair interface.** Algorithms take `[first, last)` —
  half-open, like everything in C++. `v.begin(), v.end()` until ranges
  (module 18) let you say just `v`.
- **The classics.** `std::find`, `std::max_element` (returns an
  *iterator* — mind the empty case), `std::accumulate` (in `<numeric>`;
  the init argument's *type* drives the accumulation type — `0` vs `0.0`
  matters!), `std::transform` + `std::back_inserter`.
- **The erase–remove idiom.** `std::remove` cannot erase — it only has
  iterators, no container. It *shifts the keepers forward* and returns
  the new logical end; the tail is garbage until you erase it:
  `v.erase(std::remove(...), v.end())`. **C++20's `std::erase(v, x)` /
  `std::erase_if(v, pred)`** finally wrap the dance in one call.
- **Sorted-range algorithms.** `std::binary_search`, `lower_bound`,
  `upper_bound` *require* sorted input — they don't check (that would
  defeat the O(log n) point), they just answer wrong. Precondition
  discipline is on you.

## Version notes

| Feature | Standard |
|---|---|
| `<algorithm>`, `<numeric>` core | C++98 |
| Lambdas make algorithms ergonomic | C++11 |
| `std::reduce`, parallel execution policies | C++17 |
| `std::erase` / `std::erase_if` free functions | C++20 |
| `std::ranges::` algorithm overloads + projections | C++20 (module 18) |
