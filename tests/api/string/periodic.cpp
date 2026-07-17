#include <cassert>
#include <string>
#include <vector>

#include "string/periodic.hpp"

int main() {
    assert(poe::primitive_period("ababab") == "ab");
    assert(poe::primitive_period("ababa") == "ababa");
    assert(poe::primitive_period("").empty());
    assert(
        poe::minimum_unique_repetition_counts({"aa", "a", "a", "aaa"})
        == std::vector<long long>({1, 1, 3, 2})
    );
}
