// linkage4_util.h — solution.
#ifndef CPPLINGS_LINKAGE4_UTIL_H
#define CPPLINGS_LINKAGE4_UTIL_H

// constexpr would be even better here (it implies inline, and this is a
// pure computation) — but plain inline is the general-purpose answer:
inline int clamp01(int x) {
    if (x < 0) return 0;
    if (x > 1) return 1;
    return x;
}

#endif  // CPPLINGS_LINKAGE4_UTIL_H
