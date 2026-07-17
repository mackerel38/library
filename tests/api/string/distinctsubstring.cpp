#include <cassert>
#include <string>
#include <vector>

#include "string/distinctsubstring.hpp"
#include "string/subsequence.hpp"

int main() {
    assert(poe::count_distinct_substrings_without(std::string("ab#aba"), '#') == 5);
    assert(poe::count_distinct_substrings_without(std::vector<int>{1, 2, 0, 1}, 0) == 3);
    assert(poe::count_substrings_without_subsequence(std::string("aaaaa"), std::string("a")) == 0);
    assert(poe::count_substrings_without_subsequence(std::string("abc"), std::string("ac")) == 5);
}
