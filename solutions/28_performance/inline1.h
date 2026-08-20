// inline1.h — SOLUTION header
#ifndef CPPLINGS_INLINE1_H
#define CPPLINGS_INLINE1_H

// `inline` = "this definition appears in several TUs on purpose;
// linker, fold them into one." Now every TU can see the body (so the
// optimizer can inline it) AND the build links.
inline double mean(const double* xs, int n) {
    double sum = 0.0;
    for (int i = 0; i < n; ++i) {
        sum += xs[i];
    }
    return n > 0 ? sum / n : 0.0;
}

// Defined in inline1_stats.cpp — one TU, so a plain declaration.
double spread(const double* xs, int n);

#endif
