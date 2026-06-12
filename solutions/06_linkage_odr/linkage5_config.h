// linkage5_config.h — solution: an inline variable (C++17).
#ifndef CPPLINGS_LINKAGE5_CONFIG_H
#define CPPLINGS_LINKAGE5_CONFIG_H

// inline: every TU may define it; the linker folds them into ONE object.
// constexpr makes it a compile-time constant as well.
inline constexpr double kPi = 3.14159265358979;

#endif  // CPPLINGS_LINKAGE5_CONFIG_H
