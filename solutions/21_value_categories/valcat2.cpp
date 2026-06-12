// valcat2.cpp — solution

#include <cassert>
#include <vector>

// int& : the call expression is now an lvalue aliasing the element.
int& brightest_pixel(std::vector<int>& image) {
    std::size_t best = 0;
    for (std::size_t i = 1; i < image.size(); ++i) {
        if (image[i] > image[best]) best = i;
    }
    return image[best];
}

int main() {
    std::vector<int> image = {18, 240, 77, 240, 3};

    assert(brightest_pixel(image) == 240);

    brightest_pixel(image) = 120;
    assert(image == (std::vector<int>{18, 120, 77, 240, 3}));

    brightest_pixel(image) /= 2;
    assert(image == (std::vector<int>{18, 120, 77, 120, 3}));
    return 0;
}
