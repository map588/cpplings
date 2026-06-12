// vector2.cpp — solution

#include <cassert>
#include <vector>

void drop_failing_grades(std::vector<int>& grades) {
    for (auto it = grades.begin(); it != grades.end(); /* in the body */) {
        if (*it < 60) {
            it = grades.erase(it);   // erase advances for us
        } else {
            ++it;
        }
    }
    // The C++20 spelling of this entire function:
    //     std::erase_if(grades, [](int g) { return g < 60; });
}

int main() {
    std::vector<int> grades = {55, 58, 90, 72, 40, 45, 88};
    drop_failing_grades(grades);

    assert(grades == (std::vector<int>{90, 72, 88}));
    return 0;
}
