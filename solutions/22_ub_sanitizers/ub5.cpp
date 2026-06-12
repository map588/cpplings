// ub5.cpp — solution

#include <cassert>
#include <vector>

int average_above(const std::vector<int>& readings, int threshold) {
    int sum = 0;
    int count = 0;
    for (int r : readings) {
        if (r > threshold) {
            sum += r;
            ++count;
        }
    }
    return count == 0 ? 0 : sum / count;   // the empty case, decided
}

int main() {
    std::vector<int> temps = {18, 31, 24, 35};

    assert(average_above(temps, 30) == 33);
    assert(average_above(temps, 100) == 0);
    return 0;
}
