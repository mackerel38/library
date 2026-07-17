#include "structure/sparsetable.hpp"

int minimum(int left, int right) {
    return std::min(left, right);
}

std::string concatenate(const std::string& left, const std::string& right) {
    return left + right;
}

int main() {
    poe::sparsetable<int, minimum> minimums({4, 1, 7, 2, 5});
    assert(minimums.size() == 5);
    assert(minimums[2] == 7);
    assert(minimums.prod(0, 5) == 1);
    assert(minimums.prod(2, 4) == 2);

    poe::disjointsparsetable<std::string, concatenate> words({"a", "bc", "d", "ef"});
    assert(words.size() == 4);
    assert(words[1] == "bc");
    assert(words.prod(0, 4) == "abcdef");
    assert(words.prod(1, 3) == "bcd");
    assert(words.prod(2, 3) == "d");
}
