#include "fps/combinatorial.hpp"

using namespace std;
using namespace poe;

int main() {
    mt19937 random(20260716);
    using mint = modint998244353;
    for (int trial = 0; trial < 1000; ++trial) {
        const int symbols = 1 + random() % 6;
        const int maximum = 1 + random() % 15;
        vector<int> weight(symbols);
        for (int& value : weight) value = 1 + random() % 6;
        vector<vector<mint>> count(maximum + 1, vector<mint>(symbols));
        vector<vector<mint>> length(maximum + 1, vector<mint>(symbols));
        vector<mint> expected_count(maximum + 1), expected_length(maximum + 1);
        expected_count[0] = 1;
        for (int sum = 1; sum <= maximum; ++sum) {
            for (int last = 0; last < symbols; ++last) {
                if (weight[last] > sum) continue;
                const int previous_sum = sum - weight[last];
                if (previous_sum == 0) {
                    count[sum][last] += 1;
                    length[sum][last] += 1;
                }
                for (int previous = 0; previous < symbols; ++previous) {
                    if (previous == last) continue;
                    count[sum][last] += count[previous_sum][previous];
                    length[sum][last] += length[previous_sum][previous]
                        + count[previous_sum][previous];
                }
                expected_count[sum] += count[sum][last];
                expected_length[sum] += length[sum][last];
            }
        }
        const auto actual = adjacent_distinct_words<998244353>(maximum, weight);
        assert(vector<mint>(actual.count.begin(), actual.count.end()) == expected_count);
        assert(vector<mint>(actual.length_sum.begin(), actual.length_sum.end()) == expected_length);
    }
}
