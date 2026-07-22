#include "string/all.hpp"

int main() {
    const std::string text = "mississippi";
    const auto suffix = poe::suffix_array(text);
    const std::vector<int> expected_suffix = {10, 7, 4, 1, 0, 9, 8, 6, 3, 5, 2};
    assert(suffix == expected_suffix);
    assert(poe::lcp_array(text, suffix) ==
           std::vector<int>({1, 1, 4, 0, 0, 1, 0, 2, 1, 3}));
    assert(poe::z_algorithm(std::string("ababa")) ==
           std::vector<int>({5, 0, 3, 0, 1}));
    assert(poe::suffix_array(std::vector<int>{2, 1, 2, 1}, 2) ==
           std::vector<int>({3, 1, 2, 0}));
    assert(poe::prefix_function(std::string("ababa")) ==
           std::vector<int>({0, 0, 1, 2, 3}));
    assert(poe::find_all(std::string("ababa"), std::string("aba")) ==
           std::vector<int>({0, 2}));
    assert(poe::manacher_odd(std::string("abacaba"))[3] == 4);
    assert(poe::manacher_even(std::string("abba"))[2] == 2);
    assert(poe::lcp_array(std::string{}, std::vector<int>{}).empty());
    poe::suffixquery query("banana");
    assert(query.lcp(1, 3) == 3);
    assert(query.less(1, 4, 0, 3));
    assert(query.contains("nan") && !query.contains("apple"));
}
