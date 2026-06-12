// types1.cpp — solution

#include <cassert>

int main() {
    double ratio = 3.9;

    int silent = ratio;                         // legal, lossy, invisible
    int loud{static_cast<int>(ratio)};          // explicit, intentional truncation
    unsigned int positive{0};                   // say what you mean

    assert(silent == 3);
    assert(loud == 3);
    assert(positive == 0);
    return 0;
}
