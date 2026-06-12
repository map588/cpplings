// copy2.cpp — solution

#include <cassert>
#include <cstddef>
#include <vector>

class GradeBook {
public:
    explicit GradeBook(std::size_t n) : grades_(n) {}

    // Rule of zero: no destructor, no copy ops, no move ops. vector's
    // correct behavior IS this class's correct behavior.

    void set(std::size_t i, int grade) { grades_[i] = grade; }
    int get(std::size_t i) const { return grades_[i]; }
    std::size_t count() const { return grades_.size(); }

    int average() const {
        int sum = 0;
        for (int g : grades_) sum += g;
        return sum / static_cast<int>(grades_.size());
    }

private:
    std::vector<int> grades_;
};

int main() {
    GradeBook period1(4);
    period1.set(0, 90); period1.set(1, 80);
    period1.set(2, 70); period1.set(3, 100);

    GradeBook backup = period1;
    backup.set(0, 0);
    assert(period1.get(0) == 90);
    assert(period1.average() == 85);
    assert(backup.average() == 62);

    return 0;
}
