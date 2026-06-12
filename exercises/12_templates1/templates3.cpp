// templates3.cpp
//
// Inside a template, any name that depends on T is a DEPENDENT NAME, and
// the compiler must parse the template BEFORE knowing what T is (phase
// one of "two-phase lookup"). So what is this?
//
//     std::vector<T>::const_iterator
//
// For most T it's a type. But nothing stops someone specializing
// vector-of-their-thing so that ::const_iterator is a static data
// member — the grammar genuinely can't tell. The rule: a dependent
// qualified name is assumed to be a VALUE unless you say `typename`:
//
//     typename std::vector<T>::const_iterator it = v.begin();
//
// (Sibling rule you'll meet in the wild: a dependent member TEMPLATE
// needs `.template`, as in `obj.template get<0>()`. Same ambiguity,
// other keyword.)
//
// Honest modern advice: `auto it = v.begin();` makes the whole question
// disappear, and C++20 relaxed `typename` in several spots. But you WILL
// meet this error in real codebases — learn its face.
//
// Task: fix sum_every_other twice — once with typename (do it, feel the
// history), then once more however you like.

// I AM NOT DONE

#include <cassert>
#include <vector>

template <typename T>
T sum_every_other(const std::vector<T>& v) {
    T total{};
    // "missing 'typename' prior to dependent type name" — the error of
    // a generation:
    std::vector<T>::const_iterator it = v.begin();
    while (it != v.end()) {
        total += *it;
        if (it + 1 == v.end()) break;
        it += 2;
    }
    return total;
}

int main() {
    std::vector<int> ints = {1, 2, 3, 4, 5};
    assert(sum_every_other(ints) == 9);          // 1 + 3 + 5

    std::vector<double> reals = {0.5, 9.0, 1.5};
    assert(sum_every_other(reals) == 2.0);       // 0.5 + 1.5
    return 0;
}
