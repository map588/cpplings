// namespaces3.cpp — solution

#include <type_traits>
#include <vector>

using Row = std::vector<unsigned char>;

template <typename T>
using Matrix = std::vector<std::vector<T>>;

using Comparator = bool (*)(int, int);

static_assert(std::is_same_v<Row, std::vector<unsigned char>>);
static_assert(std::is_same_v<Matrix<int>, std::vector<std::vector<int>>>);
static_assert(std::is_same_v<Matrix<unsigned char>::value_type, Row>);
static_assert(std::is_same_v<Comparator, bool (*)(int, int)>);

int main() { return 0; }
