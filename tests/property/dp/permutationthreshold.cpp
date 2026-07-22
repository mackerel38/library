#include "dp/permutationthreshold.hpp"

using namespace std;
using namespace poe;

int main() {
    using mint = modint998244353;
    mt19937 random(272);
    for (int n = 0; n <= 8; ++n) {
        for (int iteration = 0; iteration < 200; ++iteration) {
            vector<int> thresholds(n);
            for (int& value : thresholds) {
                value = static_cast<int>(random() % static_cast<unsigned int>(n + 1));
            }
            vector<mint> expected(static_cast<std::size_t>(n) + 1);
            vector<int> permutation(n);
            iota(permutation.begin(), permutation.end(), 0);
            do {
                int count = 0;
                for (int index = 0; index < n; ++index) {
                    count += permutation[index] >= thresholds[index];
                }
                ++expected[count];
            } while (next_permutation(permutation.begin(), permutation.end()));
            assert(permutation_threshold_counts<998244353>(thresholds) == expected);
        }
    }
}
