// namespaces6.cpp — solution

namespace mathlib {
    namespace v1 {
        constexpr int checksum(int x) { return x % 9; }
    }
    inline namespace v2 {            // members visible through mathlib::
        constexpr int checksum(int x) { return x % 11; }
    }
}

int main() {
    static_assert(mathlib::checksum(123) == 2);
    static_assert(mathlib::v1::checksum(123) == 6);
    static_assert(mathlib::v2::checksum(123) == 2);
    return 0;
}
