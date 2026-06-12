// linkage5_other.cpp — reports where ITS kPi lives. Nothing to fix here.

#include "linkage5_config.h"

const double* other_tu_pi_address() {
    return &kPi;
}
