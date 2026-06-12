// linkage1.cpp — solution

#include <cassert>

extern int hit_count;   // declaration: the tracker TU owns the definition
void record_hit();

int main() {
    record_hit();
    record_hit();
    record_hit();
    assert(hit_count == 3);
    return 0;
}
