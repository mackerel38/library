#include <bits/stdc++.h>
#include "fps/combinatorial.hpp"
#include "fps/rational.hpp"

using namespace std;
using namespace poe;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, maximum_power;
    cin >> n >> maximum_power;
    map<int, int> frequency;
    for (int index = 0, color; index < n; ++index) {
        cin >> color;
        ++frequency[color];
    }
    vector<int> counts;
    for (const auto [color, count] : frequency) counts.push_back(count);
    const auto distribution = labeled_color_change_counts<998244353>(counts);
    vector<modint998244353> values(n), weights(distribution.begin(), distribution.end());
    iota(values.begin(), values.end(), modint998244353{});
    const auto answer = weighted_power_sums<998244353>(values, weights, maximum_power + 1);
    for (int exponent = 1; exponent <= maximum_power; ++exponent) {
        cout << answer[exponent] << (exponent == maximum_power ? '\n' : ' ');
    }
}
