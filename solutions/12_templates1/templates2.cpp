// templates2.cpp — solution

#include <cassert>
#include <string>
#include <utility>
#include <vector>

template <typename T>
class Stack {
public:
    void push(T v) { items_.push_back(std::move(v)); }
    void pop() { items_.pop_back(); }
    const T& top() const { return items_.back(); }
    std::size_t size() const { return items_.size(); }
    bool empty() const { return items_.empty(); }

private:
    std::vector<T> items_;   // rule of zero: vector does the management
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
