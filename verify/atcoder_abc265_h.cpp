#include <bits/stdc++.h>
#include "math/multiconvolution.hpp"

using namespace std;
using namespace poe;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int height, width;
    cin >> height >> width;
    const int shift = width - 2;
    const int masks = static_cast<int>(bit_ceil(static_cast<unsigned int>(width)));
    vector<vector<modint998244353>> row(2 * shift + 1,
                                        vector<modint998244353>(masks));
    for (int first = 1; first <= width; ++first) {
        for (int second = 1; second <= width; ++second) {
            if (first == second) continue;
            if (second < first) {
                ++row[shift][first - second - 1];
            } else {
                const int surreal = (first - 1) - (width - second);
                ++row[shift + surreal][0];
            }
        }
    }

    const auto distribution = sum_xor_convolution_power<998244353>(row, height);
    const int zero = height * shift;
    modint998244353 answer = 0;
    for (int sum = zero; sum < static_cast<int>(distribution.size()); ++sum) {
        for (int mask = 0; mask < masks; ++mask) {
            if (sum > zero || mask > 0) answer += distribution[sum][mask];
        }
    }
    cout << answer << '\n';
}
