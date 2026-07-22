#include <bits/stdc++.h>
#include "fps/boundedgeometric.hpp"

using namespace std;
using namespace poe;

int main() {
    mt19937 random(0);
    for (int trial = 0; trial < 5000; ++trial) {
        const int size = random() % 7;
        vector<modint998244353> values;
        vector<long long> limits(size);
        vector<int> candidates(30);
        iota(candidates.begin(), candidates.end(), 1);
        shuffle(candidates.begin(), candidates.end(), random);
        for (int i = 0; i < size; ++i) {
            values.push_back(candidates[i]);
            limits[i] = random() % 6;
        }
        int total = accumulate(limits.begin(), limits.end(), 0);
        vector<modint998244353> expected(total + 1);
        expected[0] = 1;
        int current = 0;
        for (int i = 0; i < size; ++i) {
            vector<modint998244353> next(total + 1);
            modint998244353 power = 1;
            for (int used = 0; used <= current; ++used) {
                power = 1;
                for (int take = 0; take <= limits[i]; ++take) {
                    next[used + take] += expected[used] * power;
                    power *= values[i];
                }
            }
            expected.swap(next);
            current += limits[i];
        }
        for (int degree = 0; degree <= total + 2; ++degree) {
            const auto actual = bounded_geometric_product_coefficient(
                values, limits, degree);
            assert(actual == (degree <= total ? expected[degree] : modint998244353{0}));
        }
    }
}
