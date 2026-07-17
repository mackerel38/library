#include <cassert>
#include <random>
#include <vector>

#include "dp/wildcardorder.hpp"

int main() {
    using mint = poe::staticmodint<998244353>;
    std::mt19937 random(20260716);
    for (int count = 0; count <= 4; ++count) {
        for (int length = 0; length <= 3; ++length) {
            for (int trial = 0; trial < 20; ++trial) {
                const int alphabet = 2 + random() % 3;
                std::vector<std::vector<int>> patterns(
                    count, std::vector<int>(length));
                std::vector<std::pair<int, int>> wildcard;
                for (int row = 0; row < count; ++row) {
                    for (int column = 0; column < length; ++column) {
                        patterns[row][column] = random() % (alphabet + 1) - 1;
                        if (patterns[row][column] == -1) wildcard.emplace_back(row, column);
                    }
                }
                if (wildcard.size() > 7) continue;
                long long expected = 0;
                long long assignments = 1;
                for (std::size_t i = 0; i < wildcard.size(); ++i) assignments *= alphabet;
                for (long long code = 0; code < assignments; ++code) {
                    auto filled = patterns;
                    long long value = code;
                    for (const auto& [row, column] : wildcard) {
                        filled[row][column] = value % alphabet;
                        value /= alphabet;
                    }
                    expected += std::adjacent_find(
                        filled.begin(), filled.end(),
                        [](const auto& left, const auto& right) { return !(left < right); }) ==
                        filled.end();
                }
                assert(poe::count_strictly_increasing_wildcard_sequences<mint>(
                           patterns, alphabet) == mint{expected});
            }
        }
    }
}
