#include <cassert>

#include "fps/combinatorial.hpp"

using namespace std;
using namespace poe;

int main() {
    using mint = modint998244353;
    vector<mint> factorial(11, 1);
    for (int i = 1; i <= 10; ++i) factorial[i] = factorial[i - 1] * i;
    for (int groups = 0; groups <= 5; ++groups) {
        vector<int> periods(groups);
        auto choose_period = [&](auto&& self, int index) -> void {
            if (index < groups) {
                for (int period = 1; period <= 4; ++period) {
                    periods[index] = period;
                    self(self, index + 1);
                }
                return;
            }
            for (int n = 0; n <= 10; ++n) {
                mint ordinary = 0;
                mint labeled = 0;
                vector<int> count(groups);
                auto enumerate = [&](auto&& enumerate_self, int group, int remain) -> void {
                    if (group == groups) {
                        if (remain != 0) return;
                        ++ordinary;
                        mint ways = factorial[n];
                        for (const int value : count) ways /= factorial[value];
                        labeled += ways;
                        return;
                    }
                    for (int value = 0; value <= remain; value += periods[group]) {
                        count[group] = value;
                        enumerate_self(enumerate_self, group + 1, remain - value);
                    }
                };
                enumerate(enumerate, 0, n);
                const auto actual = divisible_group_counts<998244353>(n, periods);
                assert(actual.ordinary == ordinary);
                assert(actual.labeled == labeled);
            }
        };
        choose_period(choose_period, 0);
    }

    mt19937 random(260);
    for (int trial = 0; trial < 200; ++trial) {
        const int n = random() % 9;
        const int colors = 1 + random() % 4;
        vector<int> counts(colors), label_color;
        for (int label = 0; label < n; ++label) {
            const int color = random() % colors;
            ++counts[color];
            label_color.push_back(color);
        }
        vector<mint> expected(max(1, n));
        vector<int> permutation(n);
        iota(permutation.begin(), permutation.end(), 0);
        do {
            int changes = 0;
            for (int index = 1; index < n; ++index) {
                changes += label_color[permutation[index - 1]]
                         != label_color[permutation[index]];
            }
            ++expected[changes];
        } while (next_permutation(permutation.begin(), permutation.end()));
        assert(labeled_color_change_counts<998244353>(counts) == expected);
    }
}
