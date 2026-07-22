#include "math/multiconvolution.hpp"

using namespace std;
using namespace poe;

int main() {
    using mint = modint998244353;
    mt19937 random(265);
    for (int iteration = 0; iteration < 3000; ++iteration) {
        const int sums = 1 + static_cast<int>(random() % 5);
        const int masks = 1 << (random() % 3);
        const int exponent = random() % 6;
        vector<vector<mint>> base(sums, vector<mint>(masks));
        for (auto& row : base) {
            for (auto& value : row) value = random() % 10;
        }
        vector<vector<mint>> expected(1, vector<mint>(masks));
        expected[0][0] = 1;
        for (int power = 0; power < exponent; ++power) {
            vector<vector<mint>> next(expected.size() + base.size() - 1,
                                      vector<mint>(masks));
            for (int i = 0; i < static_cast<int>(expected.size()); ++i) {
                for (int j = 0; j < sums; ++j) {
                    for (int x = 0; x < masks; ++x) {
                        for (int y = 0; y < masks; ++y) {
                            next[i + j][x ^ y] += expected[i][x] * base[j][y];
                        }
                    }
                }
            }
            expected = std::move(next);
        }
        assert(sum_xor_convolution_power<998244353>(base, exponent) == expected);
    }
}
