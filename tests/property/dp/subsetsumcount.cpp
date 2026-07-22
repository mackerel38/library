#include "dp/subsetsumcount.hpp"

using namespace std;
using namespace poe;

int main() {
    mt19937 random(267);
    for (int iteration = 0; iteration < 5000; ++iteration) {
        const int n = static_cast<int>(random() % 15);
        const int limit = static_cast<int>(random() % 31);
        vector<int> values(n);
        for (int& value : values) value = static_cast<int>(random() % 12);

        vector<modint998244353> even(limit + 1), odd(limit + 1);
        for (int mask = 0; mask < (1 << n); ++mask) {
            int sum = 0;
            for (int index = 0; index < n; ++index) {
                if (mask >> index & 1) sum += values[index];
            }
            if (sum <= limit) {
                (popcount(static_cast<unsigned int>(mask)) & 1 ? odd : even)[sum] += 1;
            }
        }
        const auto actual = subset_sum_parity_counts<998244353>(values, limit);
        assert(actual.even == even);
        assert(actual.odd == odd);
    }
}
