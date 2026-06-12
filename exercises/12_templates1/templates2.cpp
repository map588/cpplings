// templates2.cpp
//
// Class templates: one recipe, a family of classes. Stack<int> and
// Stack<std::string> share source code and NOTHING else — separate
// types, separate compiled code, no common base.
//
// Syntax notes while you templatize:
//   - members can use T freely: T top() const; std::vector<T> items_;
//   - INSIDE the class body, `Stack` already means `Stack<T>` — no
//     angle brackets needed
//   - the rule of zero (module 08) applies beautifully here: a vector
//     member means no special members to write
//
// (Where do template definitions LIVE? In headers, fully visible — the
// compiler must see the recipe to stamp out Stack<YourType> in your TU.
// That's why template-heavy libraries are header-only, and it's the
// inline/ODR rules from module 06 that make it legal.)
//
// Task: IntStack works, but main also wants a stack of strings.
// Templatize it (and rename it Stack).

// I AM NOT DONE

#include <cassert>
#include <string>
#include <vector>

class IntStack {
public:
    void push(int v) { items_.push_back(v); }
    void pop() { items_.pop_back(); }
    int top() const { return items_.back(); }
    std::size_t size() const { return items_.size(); }
    bool empty() const { return items_.empty(); }

private:
    std::vector<int> items_;
};

int main() {
    Stack<int> numbers;
    numbers.push(1);
    numbers.push(2);
    assert(numbers.top() == 2);
    numbers.pop();
    assert(numbers.top() == 1 && numbers.size() == 1);

    Stack<std::string> words;
    words.push("hello");
    words.push("world");
    assert(words.top() == "world");
    words.pop();
    words.pop();
    assert(words.empty());
    return 0;
}
