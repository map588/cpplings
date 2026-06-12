// vector1.cpp — solution

#include <cassert>
#include <vector>

int main() {
    std::vector<double> samples;
    samples.reserve(1001);              // one allocation, then stability

    samples.push_back(36.6);
    const double* baseline = &samples[0];

    for (int i = 0; i < 1000; ++i) {
        samples.push_back(36.6 + i * 0.001);
    }

    assert(*baseline == 36.6);          // no reallocation → still valid

    assert(samples.size() == 1001);
    assert(samples.capacity() == 1001);
    return 0;
}
