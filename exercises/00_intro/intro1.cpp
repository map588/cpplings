// intro1.cpp
//
// Welcome to cpplings! This exercise teaches the workflow.
//
// Every exercise verifies itself. This one uses `assert` from <cassert>:
// if the condition is false at runtime, the program aborts with a non-zero
// exit code, and the watcher shows you the failure. When the program exits
// cleanly, the exercise passes.
//
// (A note on `assert`: it's a macro, and it compiles to *nothing* when
// NDEBUG is defined — which is what `-DNDEBUG`, and most "Release" build
// configurations, do. Never put side effects inside an assert.)
//
// Task: make the program exit cleanly, then delete the "I AM NOT DONE"
// line at the bottom to move on to the next exercise.
//   - the assert passes and the program exits 0
// Constraints:
//   - don't change the assert line — change what `answer` holds


#include <cassert>

int main() {
    int answer = 0;  // TODO: the answer to life, the universe, and everything
    assert(answer == 42);
    return 0;
}
// I AM NOT DONE
