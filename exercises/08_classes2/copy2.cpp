// copy2.cpp
//
// Same disease as copy1 — manual new[]/delete[], member-wise copy, shared
// buffer, double-free. But this time DON'T write the rule of three.
//
// The RULE OF ZERO: hold resources through members that already manage
// themselves. If the {pointer, size} pair becomes a single standard-
// library member that owns its storage, there is nothing left for
// GradeBook to manage: the compiler-generated copy ctor, copy
// assignment, destructor — and both move operations, for free — are
// all correct, because the member's are.
//
// This is the modern default. The rule of three/five is for the handful
// of classes that sit at the BOTTOM of the stack and implement the
// management itself (vector, unique_ptr, a file handle wrapper). Every
// class above them: rule of zero.
//
// Bonus fact: by writing ~GradeBook() the original author also silently
// disabled move generation for this class (see special1 next) — the
// rule-of-zero rewrite doesn't just fix the bug, it makes the class
// movable.
//
// Task: convert GradeBook to the rule of zero.
//   - compiles, every assert passes, sanitizer-clean (no double-free,
//     no leak)
//   - GradeBook declares ZERO special members: no destructor, no
//     copy/move anything
//   - no `new` or `delete` remains anywhere in the class
// Constraints:
//   - main() must not change
//   - keep the public member functions' signatures and behavior

#include <cassert>
#include <cstddef>

class GradeBook {
public:
    explicit GradeBook(std::size_t n) : count_(n), grades_(new int[n]()) {}

    ~GradeBook() { delete[] grades_; }

    void set(std::size_t i, int grade) { grades_[i] = grade; }
    int get(std::size_t i) const { return grades_[i]; }
    std::size_t count() const { return count_; }

    int average() const {
        int sum = 0;
        for (std::size_t i = 0; i < count_; ++i) sum += grades_[i];
        return sum / static_cast<int>(count_);
    }

private:
    std::size_t count_;
    int* grades_;
};

int main() {
    GradeBook period1(4);
    period1.set(0, 90); period1.set(1, 80);
    period1.set(2, 70); period1.set(3, 100);

    GradeBook backup = period1;     // must be a deep, independent copy
    backup.set(0, 0);
    assert(period1.get(0) == 90);
    assert(period1.average() == 85);
    assert(backup.average() == 62);

    return 0;
}

// I AM NOT DONE
