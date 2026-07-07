// concepts1.cpp
//
// Compile this and study the FIRST error. average() was handed
// std::string, and the complaint comes from `sum / count` — a line the
// caller has never seen, phrased in terms of the template's internals.
// Module 12 taught you to archaeology your way out. C++20 says: stop
// digging, state the requirement.
//
// A CONCEPT is a named, compile-time predicate on types. Constrain a
// template with one, and a bad call fails AT THE CALL SITE:
// "constraints not satisfied ... does not satisfy 'Numeric'". The
// diagnostic names the contract instead of the wreckage.
//
// You rarely build these from raw requires-clauses: <concepts> ships
// ready-mades (std::integral, std::floating_point, std::copyable,
// std::invocable...) and they compose with || and &&. (Note bool
// counts as integral — excluding it is a one-line refinement. Not
// required today.)
//
// Task: make average() reject non-numeric element types at the call
// site, then delete the std::string call — it was always nonsense; now
// the compiler says so clearly.
//   - define a concept named Numeric, satisfied by exactly the integer
//     and floating-point types, and constrain average() with it
//   - before deleting the words call, recompile once and READ the new
//     error: short, at the call site, naming Numeric
//   - final program compiles and every assert passes
// Constraints:
//   - do not change average()'s body or any assert
//   - deleting the words call alone is not a fix: an unconstrained
//     average() must no longer exist when you're done

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
