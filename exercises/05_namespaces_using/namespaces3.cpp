// namespaces3.cpp
//
// The third face of `using`: ALIAS-DECLARATIONS (C++11) — the replacement
// for typedef.
//
//   typedef unsigned long long u64;        // old: name buried in the middle
//   using u64 = unsigned long long;        // new: name = type, reads left-to-right
//
//   typedef int (*Callback)(int, int);     // old: where's the name?!
//   using Callback = int (*)(int, int);    // new: obvious
//
// And the killer feature typedef simply cannot do — ALIAS TEMPLATES:
//
//   template <typename T>
//   using Grid = std::vector<std::vector<T>>;
//   Grid<float> heightmap;                 // = vector<vector<float>>
//
// (typedef can't be templated; pre-C++11 code faked this with structs full
// of nested ::type members — that's why old code is littered with
// `typename Foo<T>::type`.)
//
// Task: fill in the three aliases. The static_asserts define them.

// I AM NOT DONE

#include <type_traits>
#include <vector>

// 1. A 64-bit row of pixels:
using Row = void;  // TODO: std::vector of unsigned char

// 2. An alias TEMPLATE: Matrix<T> is a vector of vectors of T.
// TODO: declare it (typedef won't work here!)

// 3. A function-pointer type taking (int, int) and returning bool:
using Comparator = void;  // TODO

static_assert(std::is_same_v<Row, std::vector<unsigned char>>);
static_assert(std::is_same_v<Matrix<int>, std::vector<std::vector<int>>>);
static_assert(std::is_same_v<Matrix<unsigned char>::value_type, Row>);
static_assert(std::is_same_v<Comparator, bool (*)(int, int)>);

int main() { return 0; }
