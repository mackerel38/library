#include <cassert>
#include <string>
#include <vector>

#include "math/modint.hpp"
#include "string/avoid.hpp"

int main() {
    using mint = poe::modint998244353;
    const std::vector<std::vector<std::string>> pattern_sets{
        {"a"}, {"aa"}, {"ab", "ba"}, {"aba", "bb"}, {"aaa", "bab", "bba"}
    };
    for (const auto& patterns : pattern_sets) {
        for (int length = 0; length <= 10; ++length) {
            int brute = 0;
            for (int mask = 0; mask < (1 << length); ++mask) {
                std::string word(length, 'a');
                for (int i = 0; i < length; ++i) if (mask >> i & 1) word[i] = 'b';
                bool valid = true;
                for (const auto& pattern : patterns) {
                    if (word.find(pattern) != std::string::npos) valid = false;
                }
                brute += valid;
            }
            assert(poe::count_avoiding_words<mint>(length, patterns, 'a', 'b').val() ==
                   static_cast<unsigned>(brute));
        }
    }
}
