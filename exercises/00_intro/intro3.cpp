// intro3.cpp
//
// Two kinds of checks power this whole course:
//
//   assert(cond)         — checked at RUNTIME. Failure aborts the program.
//   static_assert(cond)  — checked at COMPILE TIME (C++11; the message
//                          argument became optional in C++17). Failure is
//                          a compile error. It costs literally nothing at
//                          runtime — the check ran before the program existed.
//
// static_assert can only check *constant expressions*. The function below is
// marked `constexpr` (C++11), which means it is ALLOWED to run at compile
// time when its arguments are compile-time constants — that's what lets the
// static_assert call it. The same function still works on runtime values,
// as the assert at the bottom shows. One definition, two execution times.
//
// Task: square() has a bug. Fix it so both the compile-time and the runtime
// checks pass.


#include <cassert>

constexpr int square(int x) {
    return x + x;
}

int main() {
    static_assert(square(3) == 9, "square(3) should be 9");

    int five = 5;             // a runtime value: static_assert can't see this
    assert(square(five) == 25);
    return 0;
}
// I AM NOT DONE
