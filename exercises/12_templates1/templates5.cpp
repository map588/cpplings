// templates5.cpp
//
// THIS EXERCISE IS ABOUT READING THE ERROR. Compile it first. Behold:
// a screenful of noise from deep inside <algorithm> or the ostream
// machinery, for a one-line mistake in THIS file.
//
// Template errors work like this: the template is fine, YOUR TYPE didn't
// meet its (unwritten) expectations, and the failure surfaces at the
// point INSIDE the template where the expectation is used. The map:
//
//   - the TOP of the error: the actual missing operation
//     ("invalid operands to binary expression ... 'const LogEntry'")
//   - the "in instantiation of ... requested here" / "required from
//     here" chain: the breadcrumb trail from there back to YOUR call
//   - the BOTTOM of the chain: your line. Start reading from the
//     breadcrumb that names a file you wrote.
//
// Here join_lines() has an unwritten expectation about T, and LogEntry
// doesn't meet it. The error will tell you exactly which operation is
// missing — in its own verbose way. The fix belongs on LogEntry, and
// module 08 taught the right shape for exactly this kind of operator.
//
// (Module 14 shows how concepts turn this archaeology into a one-line
// "constraint not satisfied" at the call site.)
//
// Task: compile it, read the error top to bottom, then fix LogEntry.
//   - before writing any code: find the missing operation named at the
//     TOP of the error, and YOUR guilty line at the bottom of the
//     breadcrumb chain
//   - both asserts pass; entries print as `level: message` per line,
//     e.g. "WARN: disk slow"
// Constraints:
//   - join_lines is innocent — don't modify it
//   - the fix lives inside LogEntry (module 08's shape, not a free
//     function dropped at namespace scope)
//   - don't change any assert

#include <cassert>
#include <sstream>
#include <string>
#include <vector>

template <typename T>
std::string join_lines(const std::vector<T>& items) {
    std::ostringstream out;
    for (const T& item : items) {
        out << item << '\n';        // the expectation your type must meet
    }
    return out.str();
}

struct LogEntry {
    std::string level;
    std::string message;
};

int main() {
    std::vector<LogEntry> log = {
        {"INFO", "started"},
        {"WARN", "disk slow"},
    };

    assert(join_lines(log) == "INFO: started\nWARN: disk slow\n");

    std::vector<int> nums = {1, 2};
    assert(join_lines(nums) == "1\n2\n");    // ints always met the bar
    return 0;
}

// I AM NOT DONE
