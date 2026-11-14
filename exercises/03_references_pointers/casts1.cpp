// casts1.cpp
//
// (Built with -Werror=old-style-cast — see exercises.toml. In this
// file every C-style cast is a compile error. That is the exercise.)
//
// C has ONE cast, `(T)x`, and it means "make this a T — whatever it
// takes". C++ kept it for compatibility and then split its jobs into
// NAMED casts, so the reader — and the compiler — can see which job
// you meant:
//
//   static_cast<T>(x)        a conversion the type system knows about:
//                            int → double, double → int, enum ↔ int...
//                            Checked at compile time. The everyday one.
//   const_cast<T>(x)         adds or removes const (or volatile).
//                            Nothing else — and no other cast may.
//   reinterpret_cast<T>(x)   "look at these bits as a T": pointer ↔
//                            integer, pointer ↔ unrelated pointer.
//                            The dangerous one, so it is the ugly one.
//   dynamic_cast<T>(x)       a checked downcast in a class hierarchy
//                            (module 11).
//
// Why C-style casts are banned in modern code: `(T)x` tries const_cast,
// then static_cast, then static_cast plus const_cast, then
// reinterpret_cast, then reinterpret_cast plus const_cast — and takes
// the FIRST that compiles. You cannot tell from the call site which one
// you got, and a type that changes three files away can turn a
// harmless value conversion into a bit reinterpretation. Silently.
//
// A named cast refuses any job but its own: static_cast will not drop
// a const; const_cast will not change a type. So the right name is
// also a compile-time proof of what the cast does.
//
// One rule about const_cast, because it is the one people abuse: it is
// legitimate for calling an old API that forgot to say `const` but
// never writes. WRITING through it to an object that was BORN const is
// undefined behavior — the compiler may have put that object in
// read-only memory, and const_cast does not move it out.
//
// Four C-style casts below, doing four different jobs. Three need the
// right name. The fourth one was hiding a bug — no cast is its fix.
//
// Task: replace every C-style cast so the file compiles and passes.
//   - every assert passes; the sanitizers stay silent
//   - bumped(kLimit) returns 11, and kLimit is still 10 afterward
// Constraints:
//   - use the WEAKEST named cast that does each job (the wrong name
//     does not compile — let the compiler correct you)
//   - legacy_length is the old API: don't change it
//   - bumped keeps its signature and must not modify the caller's
//     object
//   - don't change main or the asserts

#include <cassert>
#include <cstddef>
#include <cstdint>

// An old C API. It only READS the text — but its author never wrote
// `const`, and it lives in a library we cannot edit.
std::size_t legacy_length(char* text) {
    std::size_t n = 0;
    while (text[n] != '\0') ++n;
    return n;
}

// (1) Integer division truncates; the author wanted the real ratio.
double ratio(int hits, int total) {
    return (double)hits / total;
}

// (2) Our text is const — correctly. The old API wants plain char*.
std::size_t label_length(const char* label) {
    return legacy_length((char*)label);
}

// (3) The address as a number, to check alignment.
bool is_aligned(const int* p) {
    return (std::uintptr_t)p % alignof(int) == 0;
}

// (4) "The limit, plus one." Read what this cast really does to a
// parameter that promised const.
int bumped(const int& limit) {
    int* p = (int*)&limit;
    *p += 1;
    return *p;
}

const int kLimit = 10;      // born const: may live in read-only memory

int main() {
    assert(ratio(1, 4) == 0.25);
    assert(label_length("gauge") == 5);

    int reading = 0;
    assert(is_aligned(&reading));

    assert(bumped(kLimit) == 11);
    assert(kLimit == 10);           // the caller's object: untouched
    return 0;
}

// I AM NOT DONE
