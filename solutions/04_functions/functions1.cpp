// functions1.cpp — solution

constexpr char pick(int)    { return 'i'; }
constexpr char pick(double) { return 'd'; }
constexpr char pick(long)   { return 'l'; }  // exact match ends the ambiguity

int main() {
    static_assert(pick(7) == 'i');       // exact match
    static_assert(pick(7.5) == 'd');     // exact match (7.5 is a double)

    static_assert(pick('a') == 'i');     // char promotes to int
    static_assert(pick(true) == 'i');    // bool promotes to int
    static_assert(pick(7.5f) == 'd');    // float promotes to double

    static_assert(pick(7L) == 'l');
    return 0;
}
