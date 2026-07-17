#include <algorithm>
#include <cassert>

#include "math/dirichlet.hpp"
#include "math/modint.hpp"

using namespace std;
using namespace poe;

int main() {
    using mint = modint998244353;
    constexpr int limit = 40;

    vector<mint> values(limit + 1);
    values[1] = 1;
    for (int i = 2; i <= limit; ++i) values[i] = (i * i + 3 * i + 1) % 7;
    const auto inverse = dirichlet_inverse(values);
    const auto identity = dirichlet_convolution(values, inverse);
    assert(identity[1] == mint{1});
    for (int i = 2; i <= limit; ++i) assert(identity[i] == mint{0});

    vector<mint> part = values;
    part[1] = 0;
    vector<mint> power(limit + 1);
    power[1] = 1;
    vector<mint> expected_log(limit + 1);
    for (int exponent = 1; exponent <= 6; ++exponent) {
        power = dirichlet_convolution(power, part);
        const mint coefficient = mint{exponent}.inv() * (exponent % 2 == 1 ? mint{1} : mint{-1});
        for (int i = 1; i <= limit; ++i) expected_log[i] += power[i] * coefficient;
    }
    const auto actual_log = dirichlet_log(values);
    for (int i = 1; i <= limit; ++i) assert(actual_log[i] == expected_log[i]);

    for (int mask = 1; mask < 16; ++mask) {
        vector<int> weights;
        vector<mint> frequency(31);
        for (int type = 0; type < 4; ++type) {
            if ((mask >> type) & 1) {
                const int weight = type + 2;
                weights.push_back(weight);
                ++frequency[weight];
            }
        }
        vector<long long> expected(31);
        for (int length = 1, minimum_product = 2; minimum_product <= 30; ++length, minimum_product *= 2) {
            vector<int> sequence(length);
            auto enumerate = [&](auto&& self, int position, int product) -> void {
                if (position == length) {
                    for (int shift = 1; shift < length; ++shift) {
                        bool smaller = false;
                        bool different = false;
                        for (int i = 0; i < length; ++i) {
                            const int rotated = sequence[(i + shift) % length];
                            if (rotated != sequence[i]) {
                                smaller = rotated < sequence[i];
                                different = true;
                                break;
                            }
                        }
                        if (different && smaller) return;
                    }
                    ++expected[product];
                    return;
                }
                for (int type = 0; type < static_cast<int>(weights.size()); ++type) {
                    if (product > 30 / weights[type]) continue;
                    sequence[position] = type;
                    self(self, position + 1, product * weights[type]);
                }
            };
            enumerate(enumerate, 0, 1);
        }
        const auto actual = product_necklace_counts(frequency);
        for (int product = 2; product <= 30; ++product) {
            assert(actual[product] == mint{expected[product]});
        }
    }
}
