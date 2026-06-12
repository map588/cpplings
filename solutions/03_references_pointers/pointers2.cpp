// pointers2.cpp — solution

#include <cassert>

int* load_samples(int n) {
    int* data = new int[n];
    for (int i = 0; i < n; ++i) {
        data[i] = (i + 1) * 10;
    }
    return data;
}

int average(int n) {
    int* samples = load_samples(n);
    int sum = 0;
    for (int i = 0; i < n; ++i) {
        sum += samples[i];
    }
    delete[] samples;                // free only after the last use
    return sum / n;
}

int main() {
    assert(average(4) == 25);
    return 0;
}
