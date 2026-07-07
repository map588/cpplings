// valcat3.cpp
//
// Module 03 taught lifetime extension: const T& binding a temporary
// keeps it alive. This exercise draws the EXACT boundary, because it's
// narrower than people think:
//
//   EXTENDS — the reference binds the prvalue directly:
//     const Reading& r = make_reading();
//   EXTENDS — the reference binds a MEMBER SUBOBJECT of the prvalue,
//   named right there in the initializer (the WHOLE temporary stays
//   alive to back it).
//
//   DOES NOT EXTEND — anything that reaches the reference through a
//   function return, even a function that "just returns its argument":
//   the temporary dies at the semicolon and the reference dangles.
//
//   The rule: extension happens when the compiler can SEE, in that one
//   initialization, that the reference binds the temporary (or its
//   subobject). A function call launders that visibility away — the
//   compiler can't know what the reference inside refers to, so the
//   temporary dies at the semicolon, normally.
//
// ASan will show the dangling case reading freed memory. (clang also
// warns -Wdangling-gsl on some of these — read it.)
//
// Task: fix the dangling `name` binding in main.
//   - the program runs clean under ASan/UBSan and both asserts pass
//   - `name` stays a const std::string& — no copying the string
// Constraints:
//   - do not change Reading, make_reading, or pick_name
//   - no new named variables in main — the fix is in `name`'s
//     initializer, using one of the extending forms above

#include <cassert>
#include <string>

struct Reading {
    std::string sensor_name;
    double value;
};

Reading make_reading() {
    return {"thermocouple-array-7-bridge-northeast", 36.6};
}

// Just forwards a reference — and breaks the extension chain doing it.
const std::string& pick_name(const Reading& r) {
    return r.sensor_name;
}

int main() {
    // Extension, the legitimate forms:
    const Reading& whole = make_reading();
    assert(whole.value == 36.6);

    // The laundered form — temporary dies at the semicolon:
    const std::string& name = pick_name(make_reading());
    assert(name == "thermocouple-array-7-bridge-northeast");
    return 0;
}

// I AM NOT DONE
