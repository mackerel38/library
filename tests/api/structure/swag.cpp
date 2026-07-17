#include "structure/swag.hpp"

std::string concatenate(const std::string& left, const std::string& right) {
    return left + right;
}

std::string empty_string() {
    return {};
}

int main() {
    poe::swag<std::string, concatenate, empty_string> queue;
    assert(queue.empty() && queue.prod().empty());
    queue.push("ab");
    queue.push("c");
    assert(queue.size() == 2 && queue.front() == "ab" && queue.back() == "c");
    assert(queue.prod() == "abc");
    queue.pop();
    queue.push("de");
    assert(queue.front() == "c" && queue.back() == "de");
    assert(queue.prod() == "cde");
}
