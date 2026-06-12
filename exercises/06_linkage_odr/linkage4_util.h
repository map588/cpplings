// linkage4_util.h — a function DEFINED in a header. Fix me.
#ifndef CPPLINGS_LINKAGE4_UTIL_H
#define CPPLINGS_LINKAGE4_UTIL_H

// Included by two .cpp files → defined in two TUs → duplicate symbol.
int clamp01(int x) {
    if (x < 0) return 0;
    if (x > 1) return 1;
    return x;
}

#endif  // CPPLINGS_LINKAGE4_UTIL_H
