// references3.cpp — solution

#include <cassert>
#include <string>

void swap_ints(int& a, int& b) {
    int tmp = a;
    a = b;
    b = tmp;
}

// In/out parameter → non-const reference.
std::size_t shout(std::string& text) {
    text += '!';
    return text.size();
}

int main() {
    int x = 1, y = 2;
    swap_ints(x, y);
    assert(x == 2 && y == 1);

    std::string word = "go";
    std::size_t len = shout(word);
    assert(word == "go!");
    assert(len == 3);
    return 0;
}
