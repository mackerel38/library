#include <cassert>
#include <random>
#include <vector>
#include "dp/subsetsum.hpp"

using namespace std;
using namespace poe;

int main() {
    mt19937 random(20260715);
    for (int n = 0; n <= 16; ++n) {
        for (int repetition = 0; repetition < 12; ++repetition) {
            vector<long long> values(n);
            for (long long& value : values) value = random() % 30;
            const long long limit = random() % 200;
            vector<unsigned long long> count(n + 1);
            vector<__int128_t> sum(n + 1);
            for (int mask = 0; mask < (1 << n); ++mask) {
                long long current = 0;
                for (int bit = 0; bit < n; ++bit) {
                    if (mask >> bit & 1) current += values[bit];
                }
                if (current < limit) {
                    const int size = popcount(static_cast<unsigned int>(mask));
                    ++count[size];
                    sum[size] += current;
                }
            }
            const auto result = subset_sum_less_by_size(values, limit);
            assert(result.count == count && result.sum == sum);
        }
    }
}
