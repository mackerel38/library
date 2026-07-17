#include "string/palindromictree.hpp"

int main() {
    poe::palindromictree<char> tree(std::string("abacaba"));
    assert(tree.length() == 7);
    assert(tree.distinct_palindromes() == 7);
    const auto longest = tree.longest_palindrome();
    assert(longest.begin == 0 && longest.end == 7 && longest.length() == 7);
    const int suffix = tree.suffix_state(4);
    const auto suffix_symbols = tree.palindrome(suffix);
    assert(!suffix_symbols.empty());
    const auto partition = tree.minimum_partition();
    assert(partition.count == 1 && partition[0] == std::make_pair(0, 7));

    poe::palindromictree<char> repeated(std::string("aaaa"));
    assert(repeated.distinct_palindromes() == 4);
    for (int state = 2; state < repeated.size(); ++state) {
        const int length = repeated[state].length;
        assert(repeated[state].occurrence == 5 - length);
    }
}
