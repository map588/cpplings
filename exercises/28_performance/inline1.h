// inline1.h — shared by inline1.cpp and inline1_stats.cpp
#ifndef CPPLINGS_INLINE1_H
#define CPPLINGS_INLINE1_H

// Small enough that every caller wants it inlined — which is why it
// lives in the header, where every TU can SEE the body. (An optimizer
// can only inline code it can see; a definition hidden in another
// .cpp is invisible until -flto.)
//
// But a definition in a header gets compiled into EVERY .cpp that
// includes it...
double mean(const double* xs, int n) {
    double sum = 0.0;
    for (int i = 0; i < n; ++i) {
        sum += xs[i];
    }
    return n > 0 ? sum / n : 0.0;
}

// Defined in inline1_stats.cpp — one TU, so a plain declaration.
double spread(const double* xs, int n);

#endif
