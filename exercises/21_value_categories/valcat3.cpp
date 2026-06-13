// valcat3.cpp
//
// Module 03 taught lifetime extension: const T& binding a temporary
// keeps it alive. This exercise draws the EXACT boundary, because it's
// narrower than people think:
//
//   EXTENDS — binding directly to the prvalue:
//     const Reading& r = make_reading();
//   EXTENDS — binding to a MEMBER of the prvalue (the WHOLE temporary
//   stays alive to back it):
//     const std::string& s = make_reading().sensor_name;
//
//   DOES NOT EXTEND — anything through a function return, even a
//   function that "just returns its argument":
//     const std::string& s = pick_name(make_reading());   // DANGLING
//
//   The rule: extension happens when the compiler can SEE, in that one
//   initialization, that the reference binds the temporary (or its
//   subobject). A function call launders that visibility away — the
//   compiler can't know what the reference inside refers to, so the
//   temporary dies at the semicolon, normally.
//
// ASan will show the third case reading freed memory. (clang also
// warns -Wdangling-gsl on some of these — read it.)
//
// Task: fix main's dangling case by binding to the member directly —
// the extending form.

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
