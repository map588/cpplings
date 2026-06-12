// raii1.cpp — solution

#include <cassert>

// --- a fake device API (pretend it's some C library) ------------------
inline int open_handles = 0;
int acquire_handle()        { ++open_handles; return 7; }
void release_handle(int)    { --open_handles; }
int read_sample(int, int i) { return i == 3 ? -1 : i * 10; }
// ----------------------------------------------------------------------

class HandleGuard {
public:
    HandleGuard() : h_(acquire_handle()) {}

    ~HandleGuard() { release_handle(h_); }   // every path, every time

    HandleGuard(const HandleGuard&) = delete;
    HandleGuard& operator=(const HandleGuard&) = delete;

    int get() const { return h_; }

private:
    int h_;
};

int process(int n) {
    HandleGuard h;                // released at }, whichever } that is
    int sum = 0;
    for (int i = 0; i < n; ++i) {
        int s = read_sample(h.get(), i);
        if (s < 0) {
            return -1;            // guard's destructor runs right here
        }
        sum += s;
    }
    return sum;
}

int main() {
    assert(process(3) == 30);
    assert(open_handles == 0);

    assert(process(5) == -1);
    assert(open_handles == 0);
    return 0;
}
