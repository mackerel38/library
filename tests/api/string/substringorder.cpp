#include "string/substringorder.hpp"

int main() {
    const std::vector<std::string> strings{"aba", "a"};
    const poe::substringorder order(strings);
    assert(order.size() == 7);

    const auto first = order.kth(0);
    assert(strings[first.string_index].substr(first.left, first.right - first.left) ==
           "a");
    const auto last = order[order.size() - 1];
    assert(strings[last.string_index].substr(last.left, last.right - last.left) ==
           "ba");

    const poe::substringorder empty(std::vector<std::string>{"", ""});
    assert(empty.size() == 0);
}
