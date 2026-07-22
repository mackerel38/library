#include <cassert>
#include <string>
#include <vector>

#include "math/modint.hpp"
#include "string/avoid.hpp"

int main() {
    using mint = poe::modint998244353;
    assert(poe::count_avoiding_words<mint>(0, std::vector<std::string>{"aa"}, 'a', 'b').val() == 1);
    assert(poe::count_avoiding_words<mint>(3, std::vector<std::string>{"aa"}, 'a', 'b').val() == 5);
    assert(poe::count_avoiding_words<mint>(4, std::vector<std::string>{"ab", "ba"}, 'a', 'b').val() == 2);
    assert((poe::minimum_pattern_replacements(
        std::string("abcdefghijklmn"),
        std::vector<std::string>{"abcd", "ijk", "ghi"}) == std::vector<int>{3, 8}));
    assert(poe::minimum_pattern_replacements(
        std::string("atcoderbeginnercontest"),
        std::vector<std::string>{"abc"}).empty());
    assert(poe::minimum_pattern_replacements(
        std::string("aaaaaaaaa"),
        std::vector<std::string>{"aa", "xyz"}).size() == 4);
}
