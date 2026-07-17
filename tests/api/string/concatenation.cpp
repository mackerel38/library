#include <cassert>
#include "string/concatenation.hpp"

int main() {
    const std::vector<std::string> strings{"abc", "ac", "ahc"};
    assert((poe::concatenation_order(strings) == std::vector<int>{0, 1, 2}));
    assert(poe::second_concatenation(strings) == "abcahcac");
    assert(poe::second_concatenation({"a", "aaa", "a", "aaaa"}) == "aaaaaaaaa");
    assert(poe::minimum_repeated_concatenation({"abc", "xxx", "abc"}, 2) == "abcabc");
    assert(poe::minimum_repeated_concatenation({"cba", "cb", "c"}, 2) == "cbac");
    assert(poe::minimum_repeated_concatenation({"a"}, 0).empty());
}
