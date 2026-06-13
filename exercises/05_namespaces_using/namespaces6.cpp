// namespaces6.cpp
//
// `inline namespace` (C++11): the members of an inline namespace are
// visible THROUGH the parent, as if declared in both. It exists for
// VERSIONING:
//
//   namespace lib {
//       namespace v1        { int checksum(int); }   // frozen
//       inline namespace v2 { int checksum(int); }   // current
//   }
//
//   lib::checksum(x)       → v2 (the inline one wins the lookup)
//   lib::v1::checksum(x)   → still reachable, old callers keep working
//
// The trick over a plain `using namespace v2;`: an inline namespace's
// contents are treated as part of the parent for things lookup-tricks
// can't fake — template specialization, and (the real reason) the MANGLED
// SYMBOL NAME still says v2. Your standard library does exactly this —
// libc++'s std::string is really std::__1::basic_string — so binaries
// built against different library versions fail to LINK instead of
// corrupting memory at runtime. ABI safety via namespaces.
//
// Task: v2 is supposed to be the default, but nobody declared it inline.

namespace mathlib {
    namespace v1 {
        constexpr int checksum(int x) { return x % 9; }    // legacy
    }
    namespace v2 {
        constexpr int checksum(int x) { return x % 11; }   // current
    }
}

int main() {
    // New code reaches the current version through the parent:
    static_assert(mathlib::checksum(123) == 2);      // 123 % 11

    // Old code that pinned v1 still compiles and still means v1:
    static_assert(mathlib::v1::checksum(123) == 6);  // 123 % 9

    // And the current version is reachable explicitly too:
    static_assert(mathlib::v2::checksum(123) == 2);
    return 0;
}

// I AM NOT DONE
