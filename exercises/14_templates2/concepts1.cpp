// concepts1.cpp
//
// Compile this and study the FIRST error. average() was handed
// std::string, and the complaint comes from `sum / count` — a line the
// caller has never seen, phrased in terms of the template's internals.
// Module 12 taught you to archaeology your way out. C++20 says: stop
// digging, state the requirement.
//
//     template <typename T> concept Numeric =
//         std::integral<T> || std::floating_point<T>;
//
//     template <Numeric T>                    // constrained!
//     T average(const std::vector<T>& v) { ... }
//
// Now average(words) fails AT THE CALL SITE: "constraints not
// satisfied ... because 'std::string' does not satisfy 'Numeric'".
// The diagnostic names the contract instead of the wreckage.
//
// (<concepts> ships ready-mades: std::integral, std::floating_point,
// std::copyable, std::invocable... compose them with || and &&.
// Note bool counts as integral — excluding it is a one-line refinement:
// `&& !std::same_as<T, bool>`. Not required today.)
//
// Task: define Numeric, constrain average with it, and then DELETE the
// std::string call — it was always nonsense; now the compiler says so
// clearly.

#include <cassert>
#include <concepts>
#include <string>
#include <vector>

template <typename T>
T average(const std::vector<T>& v) {
    T sum{};
    for (const T& x : v) sum += x;
    return sum / static_cast<T>(v.size());
}

int main() {
    std::vector<int> ints = {2, 4, 6};
    assert(average(ints) == 4);

    std::vector<double> reals = {1.0, 2.0};
    assert(average(reals) == 1.5);

    // "The average of three words", demanded a unit test in 2019:
    std::vector<std::string> words = {"a", "b", "c"};
    average(words);   // delete me once the compiler rejects me PROPERLY
    return 0;
}

// I AM NOT DONE
