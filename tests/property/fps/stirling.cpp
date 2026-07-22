#include "fps/stirling.hpp"

int main() {
    using mint = poe::staticmodint<998244353>;
    std::vector<mint> first{1};
    std::vector<mint> second{1};
    for (int n = 0; n <= 500; ++n) {
        assert(poe::signed_stirling_first_row<998244353>(n) == first);
        auto unsigned_first = first;
        for (int k = 0; k <= n; ++k) {
            if ((n - k) & 1) unsigned_first[k] = -unsigned_first[k];
        }
        assert(poe::unsigned_stirling_first_row<998244353>(n) == unsigned_first);
        assert(poe::stirling_second_row<998244353>(n) == second);
        std::vector<mint> next_first(static_cast<std::size_t>(n) + 2);
        std::vector<mint> next_second(static_cast<std::size_t>(n) + 2);
        for (int k = 0; k <= n + 1; ++k) {
            if (k > 0) {
                next_first[k] += first[k - 1];
                next_second[k] += second[k - 1];
            }
            if (k <= n) {
                next_first[k] -= first[k] * n;
                next_second[k] += second[k] * k;
            }
        }
        first.swap(next_first);
        second.swap(next_second);
    }

    std::mt19937 random(247);
    for (int trial = 0; trial < 1000; ++trial) {
        const int group_count = random() % 5;
        std::vector<int> sizes(group_count);
        std::vector<mint> expected{1};
        for (int& size : sizes) {
            size = random() % 6;
            auto factor = poe::unsigned_stirling_first_row<998244353>(size);
            std::vector<mint> next(expected.size() + factor.size() - 1);
            for (int i = 0; i < static_cast<int>(expected.size()); ++i) {
                for (int j = 0; j < static_cast<int>(factor.size()); ++j) {
                    next[i + j] += expected[i] * factor[j];
                }
            }
            expected.swap(next);
        }
        assert(poe::block_permutation_cycle_counts<998244353>(sizes) == expected);
    }
}
