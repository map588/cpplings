// const2.cpp — solution

#include <cassert>

int main() {
    int score = 10;
    int backup = 99;

    // reads only, but re-aims: pointer to const.
    const int* viewer = &score;
    viewer = &backup;
    assert(*viewer == 99);

    // writes, but never re-aims: const pointer.
    int* const fixed = &score;
    *fixed = 20;
    assert(score == 20);

    // neither: const pointer to const.
    const int* const frozen = &score;
    assert(*frozen == 20);

    return 0;
}
